	#define _CRT_SECURE_NO_WARNINGS
	#include <Windows.h>
	#include "Settings.h"
	#include <list>
	#include "CysCaller.h"
	#include "NewFramework.h"
	#include "menu.hpp"


	void* Gworld;
	bool(*WorldToScreen)(APlayerController*, Vector3*, Vector3*, bool, bool);
	void(*GetBoneMatrix)(uint64_t, FMatrix*, int);
	__forceinline uintptr_t GetRawUWorld()
	{
		static uintptr_t ret = 0;
		if (!ret)
			ret = GameInternals::ResolveRelativeAddress(GameInternals::PScan(XR("48 89 05 ? ? ? ? 0F 28 D7")), 7);

		return ret;
	}

	Vector3 APlayerController::W2S(Vector3 WorldLocation)
	{
		Vector3 Null(0.f, 0.f, 0.f);
		Vector3 ScreenLocation;
		bool ReturnValue = SP(WorldToScreen, (APlayerController*)this, &WorldLocation, &ScreenLocation, true, false);

		return ReturnValue ? ScreenLocation : Null;
	}


	Vector3 AFortPawn::GetBoneLocation(int BoneID)
	{
		FMatrix Matrix;
		SP(GetBoneMatrix, this->Mesh(), &Matrix, BoneID);
		return Vector3(Matrix.M[3][0], Matrix.M[3][1], Matrix.M[3][2]);

	}


	static void CrossHair(float X, float Y)
	{
		Canvas->DrawLine(Vector2((X / 2) - Settings::CrosshairSize, Y / 2), Vector2((X / 2) + Settings::CrosshairSize, Y / 2), 2, Col_Black);
		Canvas->DrawLine(Vector2(X / 2, (Y / 2) - Settings::CrosshairSize), Vector2(X / 2, (Y / 2) + Settings::CrosshairSize), 2, Col_Black);
	}

	static void DrawLine(int x1, int y1, int x2, int y2, int thickness, FLinearColor clr) {
		if (Canvas)
			Canvas->DrawLine(Vector2(x1, y1), Vector2(x2, y2), 1, clr);
	}

	static void DrawCornerBox(int x, int y, int w, int h, int thickness, FLinearColor clr) {
		DrawLine(x, y, x, y + (h / 5), thickness, clr);
		DrawLine(x, y, x + (w / 5), y, thickness, clr);
		DrawLine(x + w, y, x + w - (w / 5), y, thickness, clr);
		DrawLine(x + w, y, x + w, y + (h / 5), thickness, clr);
		DrawLine(x, y + h, x + (w / 5), y + h, thickness, clr);
		DrawLine(x, y + h, x, y + h - (h / 5), thickness, clr);
		DrawLine(x + w, y + h, x + w - (w / 5), y + h, thickness, clr);
		DrawLine(x + w, y + h, x + w, y + h - (h / 5), thickness, clr);
	}

	inline FLinearColor GetItemColor(EFortItemTier tier)
	{
		switch (tier)
		{
		case EFortItemTier::I:
			return ITEM_COLOR_TIER_WHITE;
		case EFortItemTier::II:
			return ITEM_COLOR_TIER_GREEN;
		case EFortItemTier::III:
			return ITEM_COLOR_TIER_BLUE;
		case EFortItemTier::IV:
			return ITEM_COLOR_TIER_PURPLE;
		case EFortItemTier::V:
			return ITEM_COLOR_TIER_ORANGE;
		case EFortItemTier::VI:
		case EFortItemTier::VII:
			return ITEM_COLOR_TIER_GOLD;
		case EFortItemTier::VIII:
		case EFortItemTier::IX:
		case EFortItemTier::X:
			return ITEM_COLOR_TIER_UNKNOWN;
		default:
			return ITEM_COLOR_TIER_WHITE;
		}
	}

	Vector3 OriginalLocation;
	Vector3 OriginalRotation;

	void(*GetViewPoint)(ULocalPlayer*, FMinimalViewInfo*, UINT) = nullptr;
	void GetViewPointHook(ULocalPlayer* this_LocalPlayer, FMinimalViewInfo* OutViewInfo, UINT StereoPass)
	{
		GetViewPoint(this_LocalPlayer, OutViewInfo, StereoPass);

		auto PlayerController = this_LocalPlayer->PlayerController();
		if (PlayerController->IsInputKeyDown(keys::RightClick)) {
			OutViewInfo->Loc = OriginalLocation;
			OutViewInfo->Rot = OriginalRotation;
		}
	}

	AFortPawn* AimActor;

	/*
	void(*GetPlayerViewPoint)(APlayerController*, Vector3*, Vector3*) = nullptr;
	void GetPlayerViewPointHook(APlayerController* PlayerController, Vector3* Location, Vector3* Rotation) {

		GetPlayerViewPoint(PlayerController, Location, Rotation);

		OriginalLocation = *Location;
		OriginalRotation = *Rotation;

		if (PlayerController->IsInputKeyDown(keys::RightClick))
		{
			if (Settings::Silent)
			{
				if (AimActor)
				{
					auto BoneHead = AimActor->GetBoneLocation(68);
					Vector3 VectorPos = BoneHead - *Location;
					Vector3 RetVector;

					float distance = VectorPos.Length();
					RetVector.x = -(((float)crt::acosf(VectorPos.z / distance) * (float)(180.0f / M_PI)) - 90.f);
					RetVector.y = (float)crt::atan2f(VectorPos.y, VectorPos.x) * (float)(180.0f / M_PI);

					*Rotation = RetVector;
				}
			}
		}
	}

	void HookS(ULocalPlayer* LocalPlayer, APlayerController* Controller)
	{
		{
			void** A = *(void***)(LocalPlayer);
			DWORD OldProtection;
			VirtualProtect(&A[88], 8, PAGE_EXECUTE_READWRITE, &OldProtection);
			GetViewPoint = decltype(GetViewPoint)(A[88]);
			A[88] = &GetViewPointHook;
			VirtualProtect(&A[88], 8, OldProtection, &OldProtection);
		}

		{
			void** A = *(void***)(Controller);
			DWORD OldProtection;
			VirtualProtect(&A[245], 8, PAGE_EXECUTE_READWRITE, &OldProtection);
			GetPlayerViewPoint = decltype(GetPlayerViewPoint)(A[245]);
			A[245] = &GetPlayerViewPointHook;
			VirtualProtect(&A[245], 8, OldProtection, &OldProtection);
		}
	}
	*/
	void* Orig;
	void Hook(UGameViewportClient* Viewport, UCanvas* Canvas)
	{
		Vector3 Aimto; float max = FLT_MAX; static bool menu_opened = true;

		X = Canvas->GetScreenSize().x;
		Y = Canvas->GetScreenSize().y;

		auto GWorld = (UWorld*)*(uintptr_t*)GetRawUWorld();

		auto LocalPlayer = GWorld->GameInstance()->LocalPlayers()->LocalPlayer();
		auto Controller = LocalPlayer->PlayerController();
		auto GetCamera = Controller->PlayerCameraManager()->GetCamera();
		auto AcknowledgedPawn = Controller->AcknowledgedPawn();

			auto Location = GetCamera.Loc;
			auto Rotation = GetCamera.Rot;
			Settings::FovAngle = GetCamera.Fov;

			if (Settings::ShowFovCircle)
			//	KGUI::DrawCircle(Canvas->GetScreenSize() / 2, (Settings::FovValue * Canvas->GetScreenSize().x / Settings::FovAngle) / 2, Col_Green); //Crash

			if (Settings::ShowCrosshair)
				//CrossHair(Canvas->GetScreenSize().x, Canvas->GetScreenSize().y); //Crash


			if (Controller->WasInputKeyJustPressed(keys::Insert))menu_opened = !menu_opened;
			{
				if (menu_opened)
				{
					KGUI::SetupCanvas(Canvas);
					GUI::Input::Handle();
					lit_menu();
				}
			}

		uintptr_t TargetPawn = 0;
		uintptr_t closestPawn = NULL;

		if (Settings::Bullettp) {
			TargetPawn = closestPawn;
		}
	


		if (Settings::FovSliderEnable)
			Controller->FOV(Settings::FovSlider);

	

		static UFortKismetLibrary* FortKismetLibrary;
		if (!FortKismetLibrary)
			FortKismetLibrary = UFortKismetLibrary::StaticClass();

		auto Players = FortKismetLibrary->GetFortPlayerPawns((UObject*)GWorld);

		for (int i = 0; i < Players.Num(); i++)
		{
			AFortPawn* Player = Players[i];

			if (!Player || Player == AcknowledgedPawn || !Player->RootComponent() || !Player->Mesh()) continue;
				auto bVisible = Player->WasRecentlyRendered(0.1);
			auto Head = Player->GetBoneLocation(68);
			auto Bottom = Controller->W2S(Player->GetBoneLocation(0));


			if (Settings::indicator && Player && AcknowledgedPawn)
			{
				Canvas->ADrawText(Vector2((double)Canvas->GetScreenSize().x / 2, (double)190), s2wc(XR("[ENEMY PLAYER IN YOUR AREA]")), Col_Green, true, true);
			}

			if (Settings::SnaplineESP and AcknowledgedPawn)
			{
				auto LocalBottom = Controller->W2S(AcknowledgedPawn->GetBoneLocation(0));
				Canvas->DrawLine(Vector2(LocalBottom.x, LocalBottom.y), Vector2(Bottom.x, Bottom.y), 1.f, Col_Green);
			}
		
			if (Settings::NameESP or Settings::DistanceESP)
			{
				auto PlayerState = Player->PlayerState();
				if (PlayerState)
				{
					auto name = PlayerState->GetPlayerName();
					if (name.c_str())
					{
						auto Pos = Controller->W2S(Vector3(Head.x, Head.y, Head.z + 30));

						auto dist_to_pawn = Head.DistanceFrom(Location);
						std::wstring distance;

						if (Settings::DistanceESP)
						{
							distance = L"[ ";
							distance += std::to_wstring((int)dist_to_pawn);
							distance += std::wstring(XR(L"m ] "));
						}

						if (Settings::NameESP)
							distance += name.c_str();

						Canvas->ADrawText(Vector2(Pos.x, Pos.y), distance.c_str(), Col_White, true, true);
					}
				}
			}
		


			if (Settings::BoxESP)
			{
				auto Pos = Controller->W2S(Vector3(Head.x, Head.y, Head.z + 15));
				float x = Pos.x;
				float y = Pos.y;
				float h = Bottom.y - y - 2;
				float w = h / 1.5;

				DrawCornerBox(x - (w / 2), y, w, h, 2, Col_Green);
			}

			auto Pos = Controller->W2S(Head);
			float Dist = GetCrossDistance(Pos.x, Pos.y, (Canvas->GetScreenSize().x / 2), (Canvas->GetScreenSize().y / 2));

			if (Dist < max)
			{
				float Radius = (Settings::FovValue * Canvas->GetScreenSize().x / Settings::FovAngle) / 2;

				if (Pos.x <= ((Canvas->GetScreenSize().x / 2) + Radius) && Pos.x >= ((Canvas->GetScreenSize().x / 2) - Radius) &&
					Pos.y <= ((Canvas->GetScreenSize().y / 2) + Radius) && Pos.y >= ((Canvas->GetScreenSize().y / 2) - Radius))
				{
					max = Dist;

					static bool disposal = false;
				
					if (Settings::VisCheck && !bVisible)
						disposal = true;
					else {
						Aimto = Pos;
						AimActor = Player;
					}
				}
				static UGameplayStatics* GameplayStatics = 0;
				if (!GameplayStatics)
					GameplayStatics = UGameplayStatics::StaticClass();

				static UClass* FortAthenaVehicle = 0;
				if (!FortAthenaVehicle)
					FortAthenaVehicle = UObject::FindClass(XR("Class FortniteGame.FortAthenaVehicle"));

				static UClass* FortProjectileBase = 0;
				if (!FortProjectileBase)
					FortProjectileBase = UObject::FindClass(XR("Class FortniteGame.FortProjectileBase"));

				TArray<AFortPawn*> Projectile, vehicles, weakspot, loot;

			
				if (Settings::Project)
				{
					GameplayStatics->GetAllActorsOfClass(GWorld, FortProjectileBase, &Projectile);
					for (int i = 0; i < Projectile.Num(); i++)
					{
						auto pawn = Projectile[i];
						auto loc = Controller->W2S(pawn->RootComponent()->RelativeLocation());

						if (!loc.IsNull() && loc.DistanceFrom(AcknowledgedPawn->RootComponent()->RelativeLocation()) / 100 > 100)
						{
							Canvas->ADrawText(Vector2((double)loc.x, (double)loc.y), s2wc(XR("[ PROJECTILES ]")), Col_DarkRed, true, false);
						}
					}
				}

			}
		}

		if (AcknowledgedPawn)
		{
			if (!Aimto.IsNull() && Controller->IsInputKeyDown(keys::RightClick) && Settings::Aimbot)
			{
				Syscall::mousemove(Aimto.x, Aimto.y, Canvas->GetScreenSize().x, Canvas->GetScreenSize().y, Settings::Smooth, false);
			}

			static UGameplayStatics* GameplayStatics = 0;
			if (!GameplayStatics)
				GameplayStatics = UGameplayStatics::StaticClass();
		

		
		
			if (Settings::LootESP)
			{
				static UClass* Class = 0;
				TArray<AFortPawn*> obj;

				if (!Class)
					Class = (UClass*)UObject::FindObject(XR("Class FortniteGame.FortPickup"));

				GameplayStatics->GetAllActorsOfClass(GWorld, Class, &obj);
				for (int i = 0; i < obj.Num(); i++)
				{
					auto pawn = obj[i];
					auto loc = pawn->RootComponent()->RelativeLocation();
					auto loc2 = Controller->W2S(loc);

					if (!loc2.IsNull() and loc.DistanceFrom(Location) < 4000)
					{
						auto name = FortKismetLibrary->GetDisplayName((UObject*)pawn->GetDefinition());
						auto tier = pawn->GetDefinition()->Tier();

						FLinearColor color = GetItemColor((EFortItemTier)tier);

						cFixName((char*)name.c_str());

						Canvas->ADrawText(Vector2(loc2.x, loc2.y), name.c_str(), color, true, false);
					}
				}
			}
		

			if (Settings::VehicleESP)
			{
				static UClass* Class = 0;
				TArray<AFortPawn*> obj;

				if (!Class)
					Class = (UClass*)UObject::FindObject(XR("Class FortniteGame.FortAthenaVehicle"));

				GameplayStatics->GetAllActorsOfClass(GWorld, Class, &obj);
				for (int i = 0; i < obj.Num(); i++)
				{
					auto pawn = obj[i];
					auto loc = pawn->RootComponent()->RelativeLocation();
					auto loc2 = Controller->W2S(loc);

					if (!loc2.IsNull() and loc.DistanceFrom(Location) < 4000)
						Canvas->ADrawText(Vector2(loc2.x, loc2.y), XR(L"[ Vehicle ]"), Col_Cyan, true, false);
				}
			}

			if (Settings::WeakSpotESP or Settings::WeakSpotAim)
			{
				static UClass* Class = 0;
				TArray<AFortPawn*> obj;

				if (!Class)
					Class = (UClass*)UObject::FindObject(XR("Class FortniteGame.BuildingWeakSpot"));

				GameplayStatics->GetAllActorsOfClass(GWorld, Class, &obj);
				for (int i = 0; i < obj.Num(); i++)
				{
					auto pawn = obj[i];
					auto loc = pawn->RootComponent()->RelativeLocation();
					auto loc2 = Controller->W2S(loc);

					if (!loc2.IsNull())
					{
						if (loc.DistanceFrom(Location) < 4000)
						{
							if (pawn->bActive())
							{
								if (Settings::WeakSpotESP)
									Canvas->ADrawText(Vector2(loc2.x, loc2.y), XR(L"[ WS ]"), Col_Cyan, true, false);
								if (Settings::WeakSpotAim)
									Syscall::mousemove(loc2.x, loc2.y, Canvas->GetScreenSize().x, Canvas->GetScreenSize().y, 2, Settings::WeakSpotAim);
							}
						}
					}
				}
			}

			if (Settings::ChestESP or Settings::AmmoBox)
			{
				static UClass* Class = 0;
				TArray<AFortPawn*> obj;

				if (!Class)
					Class = (UClass*)UObject::FindObject(XR("Class FortniteGame.BuildingContainer"));

				GameplayStatics->GetAllActorsOfClass(GWorld, Class, &obj);
				for (int i = 0; i < obj.Num(); i++)
				{
					auto pawn = obj[i];

					auto name = FortKismetLibrary->GetObjectName((UObject*)pawn);

					if (Settings::ChestESP)
					{
						if (strstr(name.ToString().c_str(), XR("Tiered_Chest")))
						{
							auto loc = pawn->RootComponent()->RelativeLocation();
							auto loc2 = Controller->W2S(loc);

							if (!loc2.IsNull() && loc.DistanceFrom(Location) < 1000)
								Canvas->ADrawText(Vector2(loc2.x, loc2.y), XR(L"[ Chest ]"), Col_Goldenrod, true, true);
						}
					}

					if (Settings::AmmoBox)
					{
						if (strstr(name.ToString().c_str(), XR("Tiered_Ammo")))
						{
							auto loc = pawn->RootComponent()->RelativeLocation();
							auto loc2 = Controller->W2S(loc);

							if (!loc2.IsNull() && loc.DistanceFrom(Location) < 100)
								Canvas->ADrawText(Vector2(loc2.x, loc2.y), XR(L"[ Ammobox ]"), Col_Yellow, true, true);
						}
					}
				}
			}
		}

		return ((void(*)(UGameViewportClient*, UCanvas*))(Orig))(Viewport, Canvas);
	}

	bool __forceinline Main()
	{
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);

		CachedData::Base = *(uint64_t*)(__readgsqword(0x60) + 0x10);

		GetBoneMatrix = decltype(GetBoneMatrix)(CachedData::Base + CachedData::GetBoneMatrix);
		WorldToScreen = decltype(WorldToScreen)(CachedData::Base + CachedData::WorldToScreen);
		//MessageBoxA(0, XR("GetBoneMatrix/W2s"), XR(""), 0);

		auto GWorld = (UWorld*)*(uintptr_t*)GetRawUWorld();
		//MessageBoxA(0, XR("GetRawUWorld"), XR(""), 0);

		auto World = (UWorld*)GWorld;
		auto GameInstance = World->GameInstance();
		auto LocalPlayers = GameInstance->LocalPlayers();
		auto LocalPlayer = LocalPlayers->LocalPlayer();
		auto AController = LocalPlayer->PlayerController();
		//MessageBoxA(0, XR("Offsets"), XR(""), 0);
		auto ViewportClient = LocalPlayer->ViewPortClient();
		//MessageBoxA(0, XR("Offsets"), XR(""), 0);

		UKismetStringLibrary::SetupKeys();
		auto index = 0x6E;
		//MessageBoxA(0, XR("index"), XR(""), 0);
		void** PostRender_VTable = *(void***)(ViewportClient); 
		DWORD OldProtection;
		VirtualProtect(&PostRender_VTable[index], 8, PAGE_EXECUTE_READWRITE, &OldProtection);
		Orig = decltype(Orig)(PostRender_VTable[index]);
		PostRender_VTable[index] = &Hook;
		VirtualProtect(&PostRender_VTable[index], 8, OldProtection, &OldProtection);

		return true;

	
	}


	int __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
	{
		if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		{
			AllocConsole();
			freopen("conin$", "r", stdin);
			freopen("conout$", "w", stdout);
			freopen("conout$", "w", stderr);
			char file_name[MAX_PATH];
			GetModuleFileNameA(hModule, file_name, MAX_PATH);
			LoadLibraryA(file_name);
			Main();
			return 1;
		}
		return 1;
	}
	  
	extern "C" __declspec(dllexport) LRESULT hook(uint32_t code, WPARAM wparam, LPARAM lparam)
	{
		return CallNextHookEx(0, code, wparam, lparam);
	}

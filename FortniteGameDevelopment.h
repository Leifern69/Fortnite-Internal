#pragma once
#include <cstdint>
#include "UE5Helper.h"
#include "GameScannerInternal.h"

#include "crt.h"

#include "GameEngine.h"
#include "RetAddressHide.h"
#include "sencrypt.hpp"
#include "uencrypt.h"

#define AllocSection(SectionName, X) __declspec(allocate(SectionName)) X;
#pragma section("VT", read, write)
AllocSection("VT", void* VT[500])

float X, Y;

struct FKey
{
	struct FName  KeyName;
	unsigned char UnknownData00[0x10];
};



namespace CachedData
{
	uint64_t Base;
	void* DrawLine;
	const uint32_t WorldToScreen = 0x7b59780;
	const uint32_t GetBoneMatrix = 0x15db560;

}

inline void cFixName(char* Name)
{
	for (int i = 0; Name[i] != '\0'; i++)
	{
		if (Name[i] == '_')
		{
			if (Name[i + 1] == '0' ||
				Name[i + 1] == '1' ||
				Name[i + 1] == '2' ||
				Name[i + 1] == '3' ||
				Name[i + 1] == '4' ||
				Name[i + 1] == '5' ||
				Name[i + 1] == '6' ||
				Name[i + 1] == '7' ||
				Name[i + 1] == '8' ||
				Name[i + 1] == '9')
				Name[i] = '\0';
		}
	}

	return;
}

std::string GetById(int index)
{
	typedef FString* (__fastcall* tGetNameByIndex)(int* index, FString* res);
	static tGetNameByIndex GetNameByIndex = nullptr;

	if (!GetNameByIndex)
		GetNameByIndex = decltype(GetNameByIndex)(GameInternals::PScan(XR("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 8B 01 4C 8B F2 8B F8 0F B7 D8 C1 EF 10 E8 ? ? ? ? 8D 34 9D ? ? ? ? 48 03 74 F8 ? 0F B7 06 83 F8 40 0F 82 ? ? ? ? C1 E8 06 33 DB 85 C0 0F 84 ? ? ? ? 8D 50 01 41 89 5E 08 41 3B 56 0C 7E 08 49 8B CE E8 ? ? ? ?")));

	FString result;
	SP(GetNameByIndex, &index, &result);

	if (result.c_str() == NULL) return (char*)"";

	auto tmp = result.ToString();

	char return_string[1024] = {};
	for (size_t i = 0; i < tmp.size(); i++)
	{
		return_string[i] += tmp[i];
	}
	//MessageBoxA(0, XR("GetNameByIndex"), XR(""), 0);

	cFixName(return_string);

	return std::string(return_string);

}

double GetCrossDistance(double x1, double y1, double x2, double y2)
{
	return crt::sqrtf(powf((float)(x1 - x2), (float)2) + powf((float)(y1 - y2), (float)2));
}

struct UCanvas
{
public:
	void DrawLine(Vector2 A, Vector2 B, float Thickness, FLinearColor Color)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.Canvas.K2_DrawLine"));

		struct
		{
			Vector2                                   ScreenPositionA;
			Vector2                                  ScreenPositionB;
			float                                              Thickness;
			FLinearColor                                RenderColor;
		} params;

		params.ScreenPositionA = A;
		params.ScreenPositionB = B;
		params.Thickness = Thickness;
		params.RenderColor = Color;
		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

	}


	__forceinline void ADrawText(Vector2 ScreenPosition, FString RenderText, FLinearColor RenderColor, bool centered, bool bOutlined, Vector2 scale = Vector2{ 1.f, 1.f })
	{
		struct textparams
		{
			UObject* RenderFont;
			FString RenderText;
			Vector2 ScreenPosition;
			Vector2 Scale;
			FLinearColor RenderColor;
			double Kerning;
			FLinearColor ShadowColor;
			Vector2 ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			FLinearColor OutlineColor;
		};

		static uintptr_t fn = NULL;
		static uintptr_t font = NULL;
		static bool setonce = false;

		if (!setonce)
		{
			fn = UObject::FindObject(XR("Function Engine.Canvas.K2_DrawText"));
			font = UObject::FindObject(XR("Font Roboto.Roboto"));
			setonce = true;
		}

		textparams params;
		params.RenderFont = (UObject*)font;
		params.RenderText = RenderText;
		params.ScreenPosition = ScreenPosition;
		params.Scale = scale;
		params.RenderColor = RenderColor;
		params.Kerning = false;
		params.ShadowColor = FLinearColor{ 0.0f, 0.0f, 0.0f, 0.0f };
		params.ShadowOffset = ScreenPosition;
		if (centered) { params.bCentreX = true; params.bCentreY = true; }
		else { params.bCentreX = false; params.bCentreY = false; }
		params.bOutlined = bOutlined;
		params.OutlineColor = { 0.0f, 0.0f, 0.0f, 1.f };
		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);
	}

	__forceinline Vector2 GetScreenSize() const
	{
		return { ScreenX, ScreenY };
	};
private:
	uint8_t Padding_0[0x40];
	int ScreenX;
	int ScreenY;
};

class AHUD
{
public:
	unsigned char Padding_d24Be[0x228];
	char bLostFocusPaused : 1; // 0x228(0x1)
	// No Padding Required Here
	char bShowHUD : 1; // 0x228(0x1)
	// No Padding Required Here
	char bShowDebugInfo : 1; // 0x228(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_HaB72 : 5; // 0x228(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_7vY26[0x3]; // 0x229(0x3)
	// No Padding Required Here
	int32_t CurrentTargetIndex; // 0x22c(0x4)
	// No Padding Required Here
	char bShowHitBoxDebugInfo : 1; // 0x230(0x1)
	// No Padding Required Here
	char bShowOverlays : 1; // 0x230(0x1)
	// No Padding Required Here
	char bEnableDebugTextShadow : 1; // 0x230(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_2aNx7 : 5; // 0x230(0x1)
	// No Padding Required Here
	unsigned char UnknownBuffer_8HAfk[0x7]; // 0x231(0x7)
	// No Padding Required Here
	struct TArray<struct AActor*> PostRenderedActors; // 0x238(0x10)
	// No Padding Required Here
	unsigned char UnknownBuffer_h7Nua[0x8]; // 0x248(0x8)
	// No Padding Required Here
	struct TArray<struct FName> DebugDisplay; // 0x250(0x10)
	// No Padding Required Here
	struct TArray<struct FName> ToggledDebugCategories; // 0x260(0x10)
	// No Padding Required Here
	struct UCanvas* Canvas; // 0x270(0x8)
	// No Padding Required Here
	struct UCanvas* DebugCanvas; // 0x278(0x8)
};

struct ARootComponent
{
public:
	Vector3 RelativeLocation()
	{
		return *(Vector3*)(this + 0x128);
	}
};

class UFortWeaponItemDefinition
{
public:
	FText DisplayName()
	{
		return *(FText*)(this + 0x90);

	}

	UINT8 Tier()
	{
		return *(UINT8*)(this + 0x73);

	}
};

class AFortWeapon
{
public:
	UFortWeaponItemDefinition* WeaponData()
	{
		return (UFortWeaponItemDefinition*)*(uintptr_t*)(this + 0x3F0); 

	}
};

class ADefinition
{
public:
	BYTE Tier()
	{
		return (BYTE)*(uintptr_t*)(this + 0x73);
	}

	FText DisplayName()
	{
		return *(FText*)(this + 0x90);
	}
};

class APlayerState
{
public:
	struct FString GetPlayerName()
	{
		static uintptr_t fn = 0; 
		if (!fn) 
			fn = UObject::FindObject(XR("Function Engine.PlayerState.GetPlayerName"));

		struct
		{
			struct FString                 ReturnValue;
		} params;


		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		auto ret = params.ReturnValue;
		return ret;
	}
};

struct AFortPawn
{
public:
	unsigned char bActive()
	{
		return *(unsigned char*)(this + 0x2b8);

	}

	ADefinition* GetDefinition()
	{

		//NOT DONE
		//this + PrimaryPickupItemEntry + ItemDefinition 
		return (ADefinition*)*(uint64_t*)(this + 0x328 + 0x18); //Check
	}
	Vector3 GetBoneLocation(int BoneID);
	APlayerState* PlayerState()
	{
		return (APlayerState*)*(uintptr_t*)(this + 0x2A8);
	}

	AFortWeapon* CurrentWeapon()
	{
		return (AFortWeapon*)*(uintptr_t*)(this + 0x8F0);
	}

	ARootComponent* RootComponent()
	{
		return (ARootComponent*)*(uint64_t*)(this + 0x190);
	}

	uint64_t Mesh()
	{
		return *(uint64_t*)(this + 0x310);

	}

	auto isVisible() -> bool
	{
		float fLastSubmitTime = float(this->Mesh() + 0x360);
		float fLastRenderTimeOnScreen = float(this->Mesh() + 0x368);

		const float fVisionTick = 0.06f;
		bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;

		return bVisible;
	}


	bool WasRecentlyRendered(float Tolerance)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.Actor.WasRecentlyRendered"));

		struct AActor_WasRecentlyRendered_Params
		{
			float                                              Tolerance;                                                // (Parm, ZeroConstructor, IsPlainOldData)
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		};

		AActor_WasRecentlyRendered_Params params;
		params.Tolerance = Tolerance;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		return params.ReturnValue;
	}

	
};



class APlayerCameraManager {
public:
	float GetFOVAngle()
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.PlayerCameraManager.GetFOVAngle"));

		struct keystruct
		{
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		};
		keystruct params;
		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		return params.ReturnValue;

	}

	bool GetPlayerViewPoint(Vector3* vCameraPos, Vector3* vCameraRot)
	{


		static uintptr_t GetPlayerViewPoint = 0;
		if (!GetPlayerViewPoint)
		{
			uintptr_t VTable = *(uintptr_t*)this;
			if (!VTable)  return false;

			GetPlayerViewPoint = *(uintptr_t*)(VTable + 0x7E8);
			if (!GetPlayerViewPoint)  return false;
		}

		auto fGetPlayerViewPoint = reinterpret_cast<void(__fastcall*)(uintptr_t, Vector3*, Vector3*)>(GetPlayerViewPoint);

		SP(fGetPlayerViewPoint, (uintptr_t)this, vCameraPos, vCameraRot);

		return true;

	}

	FMinimalViewInfo GetCamera()
	{
		Vector3 Loc, Rot;
		float FOV;

		this->GetPlayerViewPoint(&Loc, &Rot);
		FOV = this->GetFOVAngle();

		FMinimalViewInfo Cam;
		Cam.Loc = Loc;
		Cam.Rot = Rot;
		Cam.Fov = FOV;

		return Cam;
	}

};


AFortPawn* AcknowledgedPawn;

struct APlayerController
{
public:

	void FOV(float NewFOV)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.PlayerController.FOV"));

		struct APlayerController_FOV_Params
		{
			float NewFOV;
		};

		APlayerController_FOV_Params params;
		params.NewFOV = NewFOV;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

	}
	AFortPawn* AcknowledgedPawn()
	{
		return (AFortPawn*)*(uintptr_t*)(this + 0x330);

	}
	Vector3 W2S(Vector3 WorldLocation);

	APlayerCameraManager* PlayerCameraManager()
	{
		return (APlayerCameraManager*)*(uintptr_t*)(this + 0x340);

	}

	AHUD* MyHud()
	{
		return (AHUD*)*(uintptr_t*)(this + 0x338);
	}

	Vector3 W2S1(Vector3 WorldLocation)
	{
#define M_PI	3.14159265358979323846264338327950288419716939937510

		auto CameraManager = this->PlayerCameraManager();// Just wanted to show you w
		FMinimalViewInfo GetCamera = CameraManager->GetCamera();

		auto Location = GetCamera.Loc;
		auto Rotation = GetCamera.Rot;
		auto Fov = GetCamera.Fov;

		Vector3 output;
		float delta[3];
		float sp = 0, cp = 0, sy = 0, cy = 0, sr = 0, cr = 0;
		float axisx[3];
		float axisy[3];
		float axisz[3];
		float transformed[3];

		delta[0] = WorldLocation.x - Location.x;
		delta[1] = WorldLocation.y - Location.y;
		delta[2] = WorldLocation.z - Location.z;

		sp = crt::sinf(Rotation.x * M_PI / 180);
		cp = crt::cosf(Rotation.x * M_PI / 180);
		sy = crt::sinf(Rotation.y * M_PI / 180);
		cy = crt::cosf(Rotation.y * M_PI / 180);
		sr = crt::sinf(Rotation.z * M_PI / 180);
		cr = crt::cosf(Rotation.z * M_PI / 180);

		axisx[0] = cp * cy;
		axisx[1] = cp * sy;
		axisx[2] = sp;

		axisy[0] = sr * sp * cy - cr * sy;
		axisy[1] = sr * sp * sy + cr * cy;
		axisy[2] = -sr * cp;

		axisz[0] = -(cr * sp * cy + sr * sy);
		axisz[1] = cy * sr - cr * sp * sy;
		axisz[2] = cr * cp;

		transformed[0] = delta[0] * axisy[0] + delta[1] * axisy[1] + delta[2] * axisy[2];
		transformed[1] = delta[0] * axisz[0] + delta[1] * axisz[1] + delta[2] * axisz[2];
		transformed[2] = delta[0] * axisx[0] + delta[1] * axisx[1] + delta[2] * axisx[2];

		if (transformed[2] < 1)
			transformed[2] = 1;

		if (transformed[2] / 100 > 0)
		{
			float tmpx = 0, tmpy = 0;

			tmpx = (float)(X / 2);
			tmpy = (float)(Y / 2);

			output.x = tmpx + transformed[0] * tmpx / crt::tanf(Fov * M_PI / 360) / transformed[2];
			output.y = tmpy - transformed[1] * tmpx / crt::tanf(Fov * M_PI / 360) / transformed[2];

			return output;
		}

		return Vector3(0, 0, 0);
	}
		
	bool isPawnValid()
	{
		return this->AcknowledgedPawn();
	}

	bool IsInputKeyDown(const struct FKey& Key)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.PlayerController.IsInputKeyDown"));

		struct keystruct
		{
			struct FKey                                        Key;                                                      // (Parm)
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		};

		keystruct params;
		params.Key = Key;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		return params.ReturnValue;
	}

	bool GetMousePosition(float* LocationX, float* LocationY)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.PlayerController.GetMousePosition"));

		struct APlayerController_GetMousePosition_Params
		{
			float                                              LocationX;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
			float                                              LocationY;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		};

		APlayerController_GetMousePosition_Params params;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		if (LocationX != nullptr)
			*LocationX = params.LocationX;
		if (LocationY != nullptr)
			*LocationY = params.LocationY;

		return params.ReturnValue;
	}

	bool WasInputKeyJustPressed(const struct FKey& Key)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.PlayerController.WasInputKeyJustPressed"));

		struct keystruct
		{
			struct FKey                                        Key;                                                      // (Parm)
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		};

		keystruct params;
		params.Key = Key;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		return params.ReturnValue;
	}

};

class UGameViewportClient
{
public:
	void** VTable;
};

class ULocalPlayer {
public:
	APlayerController* PlayerController() {
		if (!this) return 0;
		return (APlayerController*)(*(uintptr_t*)(this + 0x30));
	}

	UGameViewportClient* ViewPortClient()
	{
		if (!this) return 0;
		return (UGameViewportClient*)(*(uintptr_t*)(this + 0x78));
	}
};

class ULocalPlayers {
public:
	ULocalPlayer* LocalPlayer() {
		if (!this) return 0;
		return (ULocalPlayer*)(*(uintptr_t*)(this));
	}
};

class UGameInstance {
public:
	ULocalPlayers* LocalPlayers() {
		if (!this) return 0;
		return (ULocalPlayers*)(*(uintptr_t*)(this + 0x38));
	}
};

struct UWorld
{
public:
	UGameInstance* GameInstance()
	{
		return (UGameInstance*)*(uintptr_t*)(this + 0x1B8);
	}
};

namespace keys
{
	FKey RightClick;
	FKey LeftClick;
	FKey Insert;
	FKey Tab;

	FKey F1;
	FKey F2;
	FKey F3;
	FKey F4;
	FKey F5;
	FKey F6;
	FKey F7;
	FKey F8;
	FKey F9;

	FKey W;
	FKey A;
	FKey S;
	FKey D;
	FKey SpaceBar;
	FKey LeftShift;
}
//AActors = 0x98
class UGameplayStatics
{
public:
	
	void GetAllActorsOfClass(UWorld* WorldContextObject, UClass* ActorClass, TArray<AFortPawn*>* OutActors)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.GameplayStatics.GetAllActorsOfClass"));

		struct UGameplayStatics_GetAllActorsOfClass_Params
		{
			UWorld* WorldContextObject;
			UClass* ActorClass;
			TArray<AFortPawn*> OutActors;
		};

		UGameplayStatics_GetAllActorsOfClass_Params params;
		params.WorldContextObject = WorldContextObject;
		params.ActorClass = ActorClass;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		if (OutActors != nullptr)
			*OutActors = params.OutActors;
	}
	
	static UGameplayStatics* StaticClass()
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Class Engine.GameplayStatics"));
		return (UGameplayStatics*)fn;
	}
};

class UKismetStringLibrary
{
public:
	struct FName Conv_StringToName(const struct FString& inString)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.KismetStringLibrary.Conv_StringToName"));

		struct UKismetStringLibrary_Conv_StringToName_Params
		{
			struct FString                                     inString;                                                 // (Parm, ZeroConstructor)
			struct FName                                       ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		};

		UKismetStringLibrary_Conv_StringToName_Params params;
		params.inString = inString;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		return params.ReturnValue;
	}

	static UKismetStringLibrary* StaticClass()
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Class Engine.KismetStringLibrary"));

		return (UKismetStringLibrary*)fn;
	}

	static inline void SetupKeys()
	{
		UKismetStringLibrary* KismetStringLib = UKismetStringLibrary::StaticClass();

		keys::Insert = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"Insert")) }, 0 };
		keys::Tab = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"Tab")) }, 0 };
		keys::LeftClick = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"LeftMouseButton")) }, 0 };
		keys::RightClick = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"RightMouseButton")) }, 0 };
		keys::F1 = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"F1")) }, 0 };
		keys::F2 = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"F2")) }, 0 };
		keys::F3 = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"F3")) }, 0 };
		keys::F4 = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"F4")) }, 0 };
		keys::F5 = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"F5")) }, 0 };
		keys::F6 = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"F6")) }, 0 };
		keys::F7 = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"F7")) }, 0 };
		keys::F8 = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"F8")) }, 0 };
		keys::F9 = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"F9")) }, 0 };
		keys::W = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"W")) }, 0 };
		keys::A = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"A")) }, 0 };
		keys::S = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"S")) }, 0 };
		keys::D = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"D")) }, 0 };
		keys::SpaceBar = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"SpaceBar")) }, 0 };
		keys::LeftShift = FKey{ FName{ KismetStringLib->Conv_StringToName(XR(L"LeftShift")) }, 0 };
	}

};

class UFortKismetLibrary
{
public:
	TArray<AFortPawn*> GetFortPlayerPawns(UObject* WorldContextObject)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function FortniteGame.FortKismetLibrary.GetFortPlayerPawns"));

		struct GetFortPlayerPawns_Params
		{
			UObject* WorldContextObject;
			TArray<AFortPawn*> ReturnValue;
		};

		GetFortPlayerPawns_Params params;
		params.WorldContextObject = WorldContextObject;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		return params.ReturnValue;
	}
	struct FString GetObjectName(UObject* Object)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.KismetSystemLibrary.GetObjectName"));

		struct UKismetSystemLibrary_GetObjectName_Params
		{
			class UObject* Object;                                                   // (ConstParm, Parm, ZeroConstructor, IsPlainOldData)
			struct FString                                     ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm)
		};

		UKismetSystemLibrary_GetObjectName_Params params;
		params.Object = Object;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		return params.ReturnValue;
	}

	struct FString GetDisplayName(class UObject* Object)
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Function Engine.KismetSystemLibrary.GetDisplayName"));

		struct UKismetSystemLibrary_GetDisplayName_Params
		{
			class UObject* Object;                                                   // (ConstParm, Parm, ZeroConstructor, IsPlainOldData)
			struct FString                                     ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm)
		};

		UKismetSystemLibrary_GetDisplayName_Params params;
		params.Object = Object;

		UObject::ProcessEvent((uintptr_t)this, (void*)fn, &params);

		return params.ReturnValue;
	}
	static UFortKismetLibrary* StaticClass()
	{
		static uintptr_t fn = NULL;
		if (!fn)
			fn = UObject::FindObject(XR("Class FortniteGame.FortKismetLibrary"));

		return (UFortKismetLibrary*)fn;
	}
};

__forceinline void VirtualSwapVTables(void* VTable, void* FunctionToSwap, void** pOriginal, int Index)
{
	DWORD Old;

	void* pVTableFunction = (void*)((uint64_t)VTable + Index);
	*pOriginal = *(PVOID*)(pVTableFunction);

	VirtualProtect(pVTableFunction, 8, PAGE_EXECUTE_READWRITE, &Old); 
	*(PVOID*)pVTableFunction = FunctionToSwap;
	VirtualProtect(pVTableFunction, 8, Old, &Old);
}
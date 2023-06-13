#include "UE5Helper.h"
#include "NewFramework.h"
#include "Settings.h"
bool menu_opened = true;

void lit_menu()
{
	static Vector2 pos = Vector2(700, 700);
	if (KGUI::Window("Leifern.win", &pos, Vector2{ 300.f, 490.f }, menu_opened))
	{
		static int tab = 1;
		if (KGUI::ButtonTab("Aimbot", Vector2{ 90, 25 }, tab == 0)) tab = 0;
		KGUI::NextColumn(100);
		if (KGUI::ButtonTab("Esp", Vector2{ 90, 25 }, tab == 1)) tab = 1;
		KGUI::NextColumn(200.0f);
		if (KGUI::ButtonTab("Misc", Vector2{ 90, 25 }, tab == 2)) tab = 2;
		KGUI::drawRect(Vector2(pos.x + 10, pos.y + 88), 280, 390, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		if (tab == 0)
		{
			KGUI::PushElementX(5);
			KGUI::PushElementY(55);
			KGUI::FakeElement();
			KGUI::Text((" "));
			//KGUI::TextLeft(("Aimbot"), Vector2(pos.x + 15, pos.y + 15), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), false, false);
			KGUI::Checkbox("Aimbot", &Settings::Aimbot);
			KGUI::Checkbox("Fov Circle [Broken Fov Value still work's)", &Settings::ShowFovCircle);
			KGUI::SliderFloat("Fov Value", &Settings::FovValue, 2, 360);
			KGUI::SliderFloat("Smoothing", &Settings::Smooth, 2, 360);
			KGUI::Checkbox("Crosshair (Broken)", &Settings::ShowCrosshair);
			KGUI::SliderFloat("Crosshair Size", &Settings::CrosshairSize, 4, 360);

		}
		if (tab == 1)
		{
			KGUI::PushElementX(5);
			KGUI::PushElementY(55);
			KGUI::FakeElement();
			KGUI::Text((" "));
			KGUI::TextLeft(("Visuals"), Vector2(pos.x + 15, pos.y + 65), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), false, false);
			KGUI::Checkbox("Player Box", &Settings::BoxESP);
			KGUI::Checkbox("Name ESP", &Settings::NameESP);
			KGUI::Checkbox("Player SnapLine", &Settings::SnaplineESP);

			KGUI::PushElementX(150);
			KGUI::PushElementY(55);
			KGUI::FakeElement();
			KGUI::Text((" "));
			KGUI::TextLeft(("World"), Vector2(pos.x + 170, pos.y + 65), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), false, false);
			KGUI::Checkbox("Chest ESP", &Settings::ChestESP);
			KGUI::Checkbox("AmmoBox ESP", &Settings::AmmoBox);
			KGUI::Checkbox("Loot ESP", &Settings::LootESP);
			KGUI::Checkbox("Vehicle ESP", &Settings::VehicleESP);
			

		}
		if (tab == 2)
		{
			KGUI::PushElementX(5);
			KGUI::PushElementY(55);
			KGUI::FakeElement();
			KGUI::Text((" "));
			KGUI::Checkbox("Visible Check", &Settings::VisCheck);
			KGUI::Checkbox("FOV Slider", &Settings::FovSliderEnable);
			KGUI::SliderFloat("Fov Slider Size", &Settings::FovSlider, 40, 160);
			KGUI::Checkbox("WeakSpot ESP", &Settings::WeakSpotESP);
			KGUI::Checkbox("Weakspot Aimbot", &Settings::WeakSpotAim);
			KGUI::Checkbox("Indicator", &Settings::indicator);
			//KGUI::Checkbox("Infinite Slide", &config.walk_slide);
			//KGUI::SliderFloat("Slide Time", &config.slide_time, 0, 100000);
		}
	}
}
#pragma once
#include "FortniteGameDevelopment.h"

APlayerController* PController;
UCanvas* Canvas;

namespace menu
{
	inline float width = 480;
	inline float height = 940;
	inline float tmp_y;
	inline bool isSliding = false;

	inline Vector2 mousePos;
	inline Vector2 menuPos;
	inline static Vector2 dragPos;

	__forceinline void Setup(UCanvas* c, APlayerController* p)
	{
		Canvas = c;
		PController = p;
	}

	__forceinline void DrawFilledRect(Vector2 position, Vector2 size, FLinearColor color)
	{
		for (float i = 0.0f; i < size.y; i += 1.0f)
			Canvas->DrawLine(Vector2(position.x, position.y + i), Vector2(position.x + size.x, position.y + i), 1, color);
	}

	__forceinline void DrawStrokeText(Vector2 position, FLinearColor color, const wchar_t* content, bool bCentered = true, bool bOutlined = true)
	{
		Canvas->ADrawText(position, content, color, bCentered, bOutlined);
	}

	inline void Initialize(const wchar_t* title, Vector2 pos)
	{
		float xl;
		float yl;
		PController->GetMousePosition(&xl, &yl);

		mousePos = Vector2((int)xl, (int)yl);
		if (!menuPos.x && !menuPos.y) menuPos = pos;

		DrawStrokeText(Vector2(menuPos.x, menuPos.y + 7), Col_Black, title, false, false);
		DrawFilledRect(Vector2(menuPos.x - 18, menuPos.y), Vector2(width, height), Col_Black);
		tmp_y = menuPos.y;
	}

	inline bool inArea(Vector2 area1, Vector2 area2)
	{
		if (mousePos.x >= area1.x && mousePos.x <= area2.x && mousePos.y >= area1.y && mousePos.y <= area2.y)
			return true;
		else
			return false;
	}


	inline void End()
	{
		if (!isSliding && inArea(Vector2(menuPos.x - 15, menuPos.y), Vector2(menuPos.x + width, menuPos.y + height)))
		{
			if (PController->IsInputKeyDown(keys::LeftClick))
			{
				if (dragPos.x == 0)
				{
					dragPos.x = (mousePos.x - width - menuPos.x);
					dragPos.y = (mousePos.y - height - menuPos.y);
				}
				menuPos.x = mousePos.x - width - dragPos.x;
				menuPos.y = mousePos.y - height - dragPos.y;
			}
			else dragPos = Vector2(0, 0);
		}
		else dragPos = Vector2(0, 0);
	}

	inline void Text(const wchar_t* content)
	{
		tmp_y += 30.f;
		DrawStrokeText(Vector2(menuPos.x, tmp_y), Col_Red, content, false, false);
	}
	inline void Text1(const wchar_t* content)
	{
		tmp_y += 30.f;
		DrawStrokeText(Vector2(menuPos.x, tmp_y), Col_Orange, content, false, false);
	}
	inline void Space(float tmpy = 30.f)
	{
		tmp_y += tmpy;
	}

	void DrawCircle(Vector2 pos, int radius, FLinearColor Color)
	{
#define M_PI	3.14159265358979323846264338327950288419716939937510
		double Step = M_PI * 2.0 / 1450;
		int Count = 0;
		Vector2 V[128];
		for (double a = 0; a < M_PI * 2.0; a += Step) {
			double X1 = radius * cos(a) + pos.x;
			double Y1 = radius * sin(a) + pos.y;
			double X2 = radius * cos(a + Step) + pos.x;
			double Y2 = radius * sin(a + Step) + pos.y;
			V[Count].x = X1;
			V[Count].y = Y1;
			V[Count + 1].x = X2;
			V[Count + 1].y = Y2;
			Canvas->DrawLine(Vector2{ V[Count].x, V[Count].y }, Vector2{ X2, Y2 }, 1.f, Color);
		}
	}

	inline void CheckBox(const wchar_t* content, bool& config)
	{
		float size = 18;

		tmp_y += 30.f;
		DrawStrokeText(Vector2(menuPos.x, tmp_y), FLinearColor(1.000000000f, 1.000000000f, 1.0f, 1.000000000f), content, false, false);

		auto isHovered = inArea(Vector2(menuPos.x + 195 - 2, tmp_y - 2), Vector2(menuPos.x + 195 + 19, tmp_y + 19));

		if (isHovered)
			if (PController->WasInputKeyJustPressed(keys::LeftClick)) config = !config;

		if (config)
			DrawFilledRect(Vector2{ menuPos.x + 195, tmp_y }, Vector2(size - 6, size - 6), Col_Green);
		else
			DrawFilledRect(Vector2{ menuPos.x + 195, tmp_y }, Vector2(size - 6, size - 6), Col_Red);
	}

	inline bool Button(const wchar_t* content)
	{
		float size = 18;

		tmp_y += 30.f;
		DrawStrokeText(Vector2(menuPos.x, tmp_y), FLinearColor(1.000000000f, 1.000000000f, 1.0f, 1.000000000f), content, false, false);

		auto isHovered = inArea(Vector2(menuPos.x + 195 - 2, tmp_y - 2), Vector2(menuPos.x + 195 + 19, tmp_y + 19));

		if (isHovered)
			if (PController->WasInputKeyJustPressed(keys::LeftClick)) return true;

		DrawFilledRect(Vector2{ menuPos.x + 195, tmp_y }, Vector2(size - 6, size - 6), Col_Blue);
	}

	void DrawFilledCircle(Vector2 position, float radius, FLinearColor color, bool screenCheck = true)
	{
		float smooth = 0.007f;

		double PI = 3.14159265359;
		int size = (int)(2.0f * PI / smooth) + 1;

		float angle = 0.0f;
		int i = 0;

		for (; angle < 2 * PI; angle += smooth, i++)
		{
			Canvas->DrawLine(Vector2(position.x, position.y), Vector2(position.x + crt::cosf(angle) * radius, position.y + crt::sinf(angle) * radius), 1, color);
		}
	}

	inline void Slider(const wchar_t* content, int& value, int min, int max)
	{
		isSliding = false;
		tmp_y += 30.f;
		DrawStrokeText(Vector2(menuPos.x, tmp_y), FLinearColor(255, 255, 255, 1.f), content, false, false);
		tmp_y += 30.f;

		Vector2 slider_size = Vector2{ 180, 5 };

		if (inArea(Vector2{ menuPos.x - 3.f, tmp_y - slider_size.y * 2 }, Vector2(menuPos.x + slider_size.x + 3.f, tmp_y + slider_size.y * 2)))
		{
			if (PController->IsInputKeyDown(keys::LeftClick))
			{
				value = ((mousePos.x - menuPos.x) * ((max - min) / slider_size.x)) + min;
				if (value < min) value = min;
				if (value > max) value = max;
				isSliding = true;
			}
		}

		float oneP = slider_size.x / (max - min);
		DrawFilledRect(Vector2{ menuPos.x, tmp_y - slider_size.y }, Vector2(oneP * (value - min), slider_size.y), Col_Green);
		DrawFilledCircle(Vector2{ menuPos.x + oneP * (value - min), tmp_y - slider_size.y + 2.5f }, 6.f, FLinearColor(255, 255, 255, 1.f));

		auto converted = std::to_string(value);
		DrawStrokeText(Vector2(menuPos.x + 195, tmp_y - slider_size.y - 25.f), FLinearColor(255, 255, 255, 1.f), std::wstring(converted.begin(), converted.end()).c_str(), false, false);
		tmp_y -= 10.f;
	}
}
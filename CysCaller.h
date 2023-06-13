#pragma once
#include <Windows.h>
#include "uencrypt.h"
#include <string>
#include "sencrypt.hpp"

namespace Syscall
{
	auto GetSyscallIndex(std::string ModuleName, std::string SyscallFunctionName, void* Function) -> bool
	{
		auto ModuleBaseAddress = iat(GetModuleHandleA)(ModuleName.c_str());
		if (!ModuleBaseAddress)
			ModuleBaseAddress = iat(LoadLibraryA)(ModuleName.c_str());
		if (!ModuleBaseAddress)
			return false;

		auto GetFunctionAddress = iat(GetProcAddress)(ModuleBaseAddress, SyscallFunctionName.c_str());
		if (!GetFunctionAddress)
			return false;

		auto SyscallIndex = *(DWORD*)((PBYTE)GetFunctionAddress + 4);

		*(DWORD*)((PBYTE)Function + 4) = SyscallIndex;

		return true;
	}


	extern "C"
	{
		NTSTATUS _NtUserSendInput(UINT a1, LPINPUT Input, int Size);
		SHORT _NtUserGetAsyncKeyState(int VKey);
	};

	VOID mouse_event_(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo, bool autoshoot = true)
	{
		static bool doneonce;
		if (!doneonce)
		{
			if (!GetSyscallIndex(XR("win32u.dll"), XR("NtUserSendInput"), _NtUserSendInput))
				return;
			doneonce = true;
		}

		INPUT Input[3] = { 0 };
		Input[0].type = INPUT_MOUSE;
		Input[0].mi.dx = dx;
		Input[0].mi.dy = dy;
		Input[0].mi.mouseData = dwData;
		Input[0].mi.dwFlags = dwFlags;
		Input[0].mi.time = 0;
		Input[0].mi.dwExtraInfo = dwExtraInfo;

		if (autoshoot)
		{
			Input[1].type = INPUT_MOUSE;
			Input[2].type = INPUT_MOUSE;
			Input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			Input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
			_NtUserSendInput((UINT)3, (LPINPUT)&Input, (INT)sizeof(INPUT));
		}
		else
			_NtUserSendInput((UINT)1, (LPINPUT)&Input, (INT)sizeof(INPUT));
	}

	static void mousemove(float tarx, float tary, float X, float Y, int smooth, bool autoshoot)
	{
		float ScreenCenterX = (X / 2);
		float ScreenCenterY = (Y / 2);
		float TargetX = 0;
		float TargetY = 0;

		if (tarx != 0)
		{
			if (tarx > ScreenCenterX)
			{
				TargetX = -(ScreenCenterX - tarx);
				TargetX /= smooth;
				if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
			}

			if (tarx < ScreenCenterX)
			{
				TargetX = tarx - ScreenCenterX;
				TargetX /= smooth;
				if (TargetX + ScreenCenterX < 0) TargetX = 0;
			}
		}

		if (tary != 0)
		{
			if (tary > ScreenCenterY)
			{
				TargetY = -(ScreenCenterY - tary);
				TargetY /= smooth;
				if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
			}

			if (tary < ScreenCenterY)
			{
				TargetY = tary - ScreenCenterY;
				TargetY /= smooth;
				if (TargetY + ScreenCenterY < 0) TargetY = 0;
			}
		}
		mouse_event((DWORD)MOUSEEVENTF_MOVE, (DWORD)TargetX, (DWORD)TargetY, (DWORD)0, (ULONG_PTR)0);
	}
}
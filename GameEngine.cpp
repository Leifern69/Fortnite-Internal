#include "GameEngine.h"
#include "RetAddressHide.h"
#include "GameScannerInternal.h"
#include "sencrypt.hpp"

std::string UObject::GetName()
{
	std::string name(Name.GetName());
	if (Name.Number > 0)
	{
		name += '_' + std::to_string(Name.Number);
	}

	auto pos = name.rfind('/');
	if (pos == std::string::npos)
	{
		return name;
	}

	return name.substr(pos + 1);
}

std::string UObject::GetFullName()
{
	std::string name;

	if (Class != nullptr)
	{
		std::string temp;
		for (auto p = Outer; p; p = p->Outer)
		{
			temp = p->GetName() + "." + temp;
		}

		name = Class->GetName();
		name += " ";
		name += temp;
		name += GetName();
	}

	return name;
}

void UObject::ProcessEvent(uintptr_t address, void* fnobject, void* parms)
{
	auto index = *reinterpret_cast<void***>(address);
	auto fProcessEvent = static_cast<void(*)(void* address, void* fnobject, void* parms)>(index[0x4D]);
	SP(fProcessEvent, (void*)address, (void*)fnobject, (void*)parms);
}


BOOLEAN UObject::IsA(PVOID parentClass) {
	for (auto super = this->Class; super; super = super->SuperClass) {
		if (super == parentClass) {
			return TRUE;
		}
	}

	return FALSE;
}

uintptr_t UObject::FindObject(const std::string& name)
{
	static FUObjectArray* GObjects = NULL;
	if (!GObjects)
		GObjects = (FUObjectArray*)GameInternals::ResolveRelativeAddress(GameInternals::PScan(XR("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1")), 7);

	auto GlobalObjects = GObjects->ObjObjects;
	//MessageBoxA(0, XR("GObjects"), XR(""), 0);
	for (int i = 0; i < GlobalObjects.Num(); ++i)
	{
		auto object = GlobalObjects.GetByIndex(i);

		if (object == nullptr)
		{
			continue;
		}

		if (object->GetFullName() == name)
		{
		//	std::cout << name.c_str() << " : 0x" << std::hex << object << std::endl;

			return (uintptr_t)(object);
		}
	}

	return 0;
}
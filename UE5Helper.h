#pragma once

#define ITEM_COLOR_TIER_WHITE	FLinearColor{ 255.f, 255.f, 255.f, 1 }
#define ITEM_COLOR_TIER_GREEN	FLinearColor{ 0.0f, 255.f, 0.0f, 1 }
#define ITEM_COLOR_TIER_BLUE	FLinearColor{ 9.0f, 63.0f, 150.f, 1 }
#define ITEM_COLOR_TIER_PURPLE	FLinearColor{ 154.f, 18.f, 179.f, 1 }
#define ITEM_COLOR_TIER_ORANGE	FLinearColor{ 85.0f, 65.f, 0.0f, 1 }
#define ITEM_COLOR_TIER_GOLD	FLinearColor{ 95.f, 85.f, 45.f, 1 }
#define ITEM_COLOR_TIER_UNKNOWN FLinearColor{ 1.0f, 0.0f, 1.0f, 1 }
#define ITEM_COLOR_MEDS			FLinearColor{ 9.f, 55.f, 55.f, 1 }
#define ITEM_COLOR_SHIELDPOTION FLinearColor{ 35.f, 55.f, 85.f, 1 }
#define ITEM_COLOR_CHEST		FLinearColor{95.f, 95.f, 0.0f, 1}
#define ITEM_COLOR_SUPPLYDROP	FLinearColor{ 9.f, 1.f, 1.f, 1 }

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "crt.h"

template<class T>
struct TArray
{
public:
	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline void Clear()
	{
		Data = nullptr;
		Count = Max = 0;
	}

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline const T& operator[](int i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

	inline void Add(T A)
	{
		if (!Data)
			Data = (T*)malloc(4096);

		Max = 4096 / sizeof(T);
		Data[Count] = A;
		Count++;
	}

	inline void Free()
	{
		free(Data);
		Count = Max = 0;
	}

	T* Data;
	int32_t Count;
	int32_t Max;
};

class FTextData {
public:
	char pad_0x0000[0x28];  //0x0000 //HERE 0x28
	wchar_t* Name;          //0x0028 
	__int32 Length;         //0x0030 
};

struct FText {
	FTextData* Data;
	char UnknownData[0x10];

	wchar_t* GetText() const {
		if (Data)
			return Data->Name;

		return nullptr;
	}
};

struct FString : private TArray<wchar_t>
{
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

struct Vector2
{
	double x, y;

	Vector2() : x(0.f), y(0.f)
	{

	}

	Vector2(double _x, double _y) : x(_x), y(_y)
	{

	}

	Vector2(int _x, int _y) : x(_x), y(_y)
	{

	}
	~Vector2()
	{

	}

	__forceinline bool IsNull()
	{
		return (!x && !y);
	}

	__forceinline bool IsOutOfBounds(int X, int Y)
	{
		if (x >= X || y >= Y)
			return false;
		else if (x <= 0 || y <= 0)
			return false;
	}

	__forceinline bool IsOutOfBounds(Vector2 v)
	{
		if ((x > v.x && y > v.y) || (x <= 0 && y <= 0))
			return true;

		return false;
	}

	__forceinline Vector2 operator+(Vector2 v)
	{
		return Vector2(x + v.x, y + v.y);
	}

	__forceinline Vector2 operator-(Vector2 v)
	{
		return Vector2(x - v.x, y - v.y);
	}

	__forceinline Vector2 operator+(float v)
	{
		return Vector2(x + v, y + v);
	}

	__forceinline Vector2 operator-(float v)
	{
		return Vector2(x - v, y - v);
	}

	__forceinline Vector2 operator+=(Vector2 v)
	{
		x += v.x;
		y += v.y;
		return Vector2(x, y);
	}

	__forceinline Vector2 operator-=(Vector2 v)
	{
		x -= v.x;
		y -= v.y;
		return Vector2(x, y);
	}

	__forceinline Vector2 operator/(Vector2 v)
	{
		return Vector2(x / v.x, y / v.y);
	}

	__forceinline Vector2 operator/(float v)
	{
		return Vector2(x / v, y / v);
	}

	void operator=(Vector2 _A)
	{
		x = _A.x;
		y = _A.y;
	}

	__forceinline Vector2 operator*(float flNum)
	{
		return Vector2(x * flNum, y * flNum);
	}
};

class Vector3
{
public:
	double x, y, z;

	__forceinline Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	__forceinline Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	__forceinline ~Vector3()
	{

	}

	__forceinline bool IsNull()
	{
		return (!x && !y && !z);
	}

	__forceinline double Length() {
		return sqrt(x * x + y * y + z * z);
	}

	__forceinline float DistanceFrom(const Vector3& Other) const {
		auto pawn = *this;

		float x = Other.x - pawn.x;
		float y = Other.y - pawn.y;
		float z = Other.z - pawn.z;
		
		return crt::sqrtf(x * x + y * y + z * z) / 100.0f;
	}

	__forceinline Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	__forceinline Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	__forceinline Vector3 operator+(float v)
	{
		return Vector3(x + v, y + v, z + v);
	}

	__forceinline Vector3 operator-(float v)
	{
		return Vector3(x - v, y - v, z - v);
	}

	__forceinline Vector3 operator+=(Vector3 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return Vector3(x, y, z);
	}

	__forceinline Vector3 operator-=(Vector3 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return Vector3(x, y, z);
	}

	__forceinline Vector3 operator/(Vector3 v)
	{
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	__forceinline Vector3 operator/(float v)
	{
		return Vector3(x / v, y / v, z / v);
	}

	Vector3 operator=(Vector3 _A)
	{
		x = _A.x;
		y = _A.y;
		z = _A.z;

		return Vector3(x, y, z);
	}

	__forceinline Vector3 operator*(float flNum)
	{
		return Vector3(x * flNum, y * flNum, z * flNum);
	}
};

class FMinimalViewInfo
{
public:
	Vector3 Loc;
	Vector3 Rot;
	float Fov;
};

void __forceinline AnsiToWide(char* inAnsi, wchar_t* outWide)
{
	int i = 0;
	for (; inAnsi[i] != '\0'; i++)
		outWide[i] = (wchar_t)(inAnsi)[i];
	outWide[i] = L'\0';
}

enum class EFortItemTier : uint8_t
{
	No_Tier = 0,
	I = 1,
	II = 2,
	III = 3,
	IV = 4,
	V = 5,
	VI = 6,
	VII = 7,
	VIII = 8,
	IX = 9,
	X = 10,
	NumItemTierValues = 11,
	EFortItemTier_MAX = 12
};

struct FLinearColor
{
	FLinearColor() : R(0.f), G(0.f), B(0.f), A(0.f)
	{

	}

	FLinearColor(float _R, float _G, float _B, float _A) : R(_R), G(_G), B(_B), A(_A)
	{

	}

	FLinearColor ConvertRGB(Vector3 RGB)
	{
		if (RGB.x == 0)
			RGB.x = 1;
		if (RGB.y == 0)
			RGB.y = 1;
		if (RGB.z == 0)
			RGB.z = 1;
		return { (float)RGB.x / 255.f, (float)RGB.y / 255.f, (float)RGB.z / 255.f, 1.f };
	}

	float R, G, B, A;
};

struct FMatrix
{
	double M[4][4];
};
#define Col_AliceBlue { 0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f }
#define Col_AntiqueWhite { 0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f }
#define Col_Aqua { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f }
#define Col_Aquamarine { 0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f }
#define Col_Azure { 0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f }
#define Col_Beige { 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f }
#define Col_Bisque { 1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f }
#define Col_Black { 0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f }
#define Col_BlanchedAlmond { 1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f }
#define Col_Blue { 0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f }
#define Col_BlueViolet { 0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f }
#define Col_Brown { 0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f }
#define Col_BurlyWood { 0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f }
#define Col_CadetBlue { 0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f }
#define Col_Chartreuse { 0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f }
#define Col_Chocolate { 0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f }
#define Col_Coral { 1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f }
#define Col_CornflowerBlue { 0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f }
#define Col_Cornsilk { 1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f }
#define Col_Crimson { 0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f }
#define Col_Cyan { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f }
#define Col_DarkBlue { 0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f }
#define Col_DarkCyan { 0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f }
#define Col_DarkGoldenrod { 0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f }
#define Col_DarkGray { 0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f }
#define Col_DarkGreen { 0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f }
#define Col_DarkKhaki { 0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f }
#define Col_DarkMagenta { 0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f }
#define Col_DarkOliveGreen { 0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f }
#define Col_DarkOrange { 1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f }
#define Col_DarkOrchid { 0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f }
#define Col_DarkRed { 0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f }
#define Col_DarkSalmon { 0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f }
#define Col_DarkSeaGreen { 0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f }
#define Col_DarkSlateBlue { 0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f }
#define Col_DarkSlateGray { 0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f }
#define Col_DarkTurquoise { 0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f }
#define Col_DarkViolet { 0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f }
#define Col_DeepPink { 1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f }
#define Col_DeepSkyBlue { 0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f }
#define Col_DimGray { 0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f }
#define Col_DodgerBlue { 0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f }
#define Col_Firebrick { 0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f }
#define Col_FloralWhite { 1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f }
#define Col_ForestGreen { 0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f }
#define Col_Fuchsia { 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f }
#define Col_Gainsboro { 0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f }
#define Col_GhostWhite { 0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f }
#define Col_Gold { 1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f }
#define Col_Goldenrod { 0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f }
#define Col_Gray { 0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f }
#define Col_Green { 0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f }
#define Col_GreenYellow { 0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f }
#define Col_Honeydew { 0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f }
#define Col_HotPink { 1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f }
#define Col_IndianRed { 0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f }
#define Col_Indigo { 0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f }
#define Col_Ivory { 1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f }
#define Col_Khaki { 0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f }
#define Col_Lavender { 0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f }
#define Col_LavenderBlush { 1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f }
#define Col_LawnGreen { 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f }
#define Col_LemonChiffon { 1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f }
#define Col_LightBlue { 0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f }
#define Col_LightCoral { 0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f }
#define Col_LightCyan { 0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f }
#define Col_LightGoldenrodYellow { 0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f }
#define Col_LightGreen { 0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f }
#define Col_LightGray { 0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f }
#define Col_LightPink { 1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f }
#define Col_LightSalmon { 1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f }
#define Col_LightSeaGreen { 0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f }
#define Col_LightSkyBlue { 0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f }
#define Col_LightSlateGray { 0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f }
#define Col_LightSteelBlue { 0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f }
#define Col_LightYellow { 1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f }
#define Col_Lime { 0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f }
#define Col_LimeGreen { 0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f }
#define Col_Linen { 0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f }
#define Col_Magenta { 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f }
#define Col_Maroon { 0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f }
#define Col_MediumAquamarine { 0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f }
#define Col_MediumBlue { 0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f }
#define Col_MediumOrchid { 0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f }
#define Col_MediumPurple { 0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f }
#define Col_MediumSeaGreen { 0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f }
#define Col_MediumSlateBlue { 0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f }
#define Col_MediumSpringGreen { 0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f }
#define Col_MediumTurquoise { 0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f }
#define Col_MediumVioletRed { 0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f }
#define Col_MidnightBlue { 0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f }
#define Col_MintCream { 0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f }
#define Col_MistyRose { 1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f }
#define Col_Moccasin { 1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f }
#define Col_NavajoWhite { 1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f }
#define Col_Navy { 0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f }
#define Col_OldLace { 0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f }
#define Col_Olive { 0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f }
#define Col_OliveDrab { 0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f }
#define Col_Orange { 1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f }
#define Col_OrangeRed { 1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f }
#define Col_Orchid { 0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f }
#define Col_PaleGoldenrod { 0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f }
#define Col_PaleGreen { 0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f }
#define Col_PaleTurquoise { 0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f }
#define Col_PaleVioletRed { 0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f }
#define Col_PapayaWhip { 1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f }
#define Col_PeachPuff { 1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f }
#define Col_Peru { 0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f }
#define Col_Pink { 1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f }
#define Col_Plum { 0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f }
#define Col_PowderBlue { 0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f }
#define Col_Purple { 0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f }
#define Col_Red { 1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f }
#define Col_RosyBrown { 0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f }
#define Col_RoyalBlue { 0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f }
#define Col_SaddleBrown { 0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f }
#define Col_Salmon { 0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f }
#define Col_SandyBrown { 0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f }
#define Col_SeaGreen { 0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f }
#define Col_SeaShell { 1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f }
#define Col_Sienna { 0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f }
#define Col_Silver { 0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f }
#define Col_SkyBlue { 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f }
#define Col_SlateBlue { 0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f }
#define Col_SlateGray { 0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f }
#define Col_Snow { 1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f }
#define Col_SpringGreen { 0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f }
#define Col_SteelBlue { 0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f }
#define Col_Tan { 0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f }
#define Col_Teal { 0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f }
#define Col_Thistle { 0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f }
#define Col_Tomato { 1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f }
#define Col_Transparent { 0.000000000f, 0.000000000f, 0.000000000f, 0.000000000f }
#define Col_Turquoise { 0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f }
#define Col_Violet { 0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f }
#define Col_Wheat { 0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f }
#define Col_White { 1.000000000f, 1.000000000f, 1.0f, 1.000000000f }
#define Col_WhiteSmoke { 0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f }
#define Col_Yellow { 1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f }
#define Col_YellowGreen { 0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f }
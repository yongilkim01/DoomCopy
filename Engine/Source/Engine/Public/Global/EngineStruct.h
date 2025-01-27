#pragma once

struct FDebugColor
{
public:
	float4 Color;
};

struct FPaperSpriteData
{
	float4 CuttingLocation = { 0.0f, 0.0f };
	float4 CuttingSize = { 1.0f, 1.0f };
	float4 Pivot = { 0.5f, 0.5f };
};

struct FUVValue
{
public:
	float4 PlusUVValue;
};

struct FResultColor
{
public:
	float4 PlusColor;
	float4 MulColor;
};
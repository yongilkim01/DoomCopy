#pragma once

typedef __int8 uint8;

enum class EProjectionType : uint8
{
	Perspective,
	Orthographic,
};

enum class EShaderType : uint8
{
	VS,
	PS,
	HS,
	DS,
	GS,
	CS,
	MAX,
};

enum class EEngineCameraType : uint8
{
	MainCamera,
	UICamera = 127,
};
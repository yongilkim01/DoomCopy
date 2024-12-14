#include "pch.h"
#include "DoomCore.h"

#include "Math/EngineMath.h"

CreateContentsCoreDefine(UDoomCore);

UDoomCore::UDoomCore()
{
}

UDoomCore::~UDoomCore()
{
}

void UDoomCore::EngineStart(UEngineInitData& Data)
{
	float width = GetSystemMetrics(SM_CXSCREEN) - WindowWidth;
	float height = GetSystemMetrics(SM_CYSCREEN) - WindowHeight;

	Data.WindowPosition = { width / 2.0f , height / 2.0f };
	Data.WindowSize = { WindowWidth, WindowHeight };
}

void UDoomCore::EngineTick(float DeltaTime)
{
}

void UDoomCore::EngineEnd()
{
}
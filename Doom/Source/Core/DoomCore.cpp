#include "pch.h"
#include "DoomCore.h"

#include "Math/EngineMath.h"
#include "Core/EngineCore.h"
#include "Classes/Level.h"

#pragma comment(lib, "Engine.lib")

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

	std::shared_ptr<ULevel> Level = UEngineCore::CreateLevel<>("TitleLevel");
}

void UDoomCore::EngineTick(float DeltaTime)
{
}

void UDoomCore::EngineEnd()
{
}
#include "pch.h"
#include "DoomCore.h"

#include "GameMode/TitleGameMode.h"

#include <Core/Math/EngineMath.h>
#include <Core/EngineCore.h>
#include <Classes/Engine/Level.h>
#include <GameFramework/Actor.h>

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

	UEngineCore::CreateLevel<ATitleGameMode, AActor>("TitleLevel");
	UEngineCore::OpenLevel("TitleLevel");
}

void UDoomCore::EngineTick(float DeltaTime)
{
}

void UDoomCore::EngineEnd()
{
}
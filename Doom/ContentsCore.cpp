#include "contents_pch.h"
#include "ContentsCore.h"

#include "Math/EngineMath.h"

CreateContentsCoreDefine(UContentsCore);

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}

void UContentsCore::EngineStart(UEngineInitData& Data)
{
	int width = GetSystemMetrics(SM_CXSCREEN) - WindowWidth;
	int height = GetSystemMetrics(SM_CYSCREEN) - WindowHeight;

	Data.WindowPosition = { width / 2 , height / 2 };
	Data.WindowSize = { WindowWidth, WindowHeight };
}

void UContentsCore::EngineTick(float DeltaTime)
{
}

void UContentsCore::EngineEnd()
{
}
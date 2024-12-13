#include "contents_pch.h"
#include "ContentsCore.h"

CreateContentsCoreDefine(UContentsCore);

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}

void UContentsCore::EngineStart(UEngineInitData& Data)
{
	Data.WindowPosition = { 100, 100 };
	Data.WindowSize = { 300, 300 };
}

void UContentsCore::EngineTick(float DeltaTime)
{
}

void UContentsCore::EngineEnd()
{
}
#include "pch.h"
#include "DoomCore.h"

#include "GameMode/TitleGameMode.h"

#include <Core/Math/EngineMath.h>
#include <Core/EngineCore.h>
#include <Core/Misc/DirectoryHelper.h>
#include <Core/Misc/FileHelper.h>
#include <Classes/Engine/Level.h>
#include <Classes/Engine/Texture.h>
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
	InitWindowSize(Data);

	{
		FDirectoryHelper DirectoryHelper;
		if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
			return;
		}
		std::vector<FFileHelper> ImageFiles = DirectoryHelper.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });

		for (int i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UTexture::Load(FilePath);
		}
	}

	UEngineCore::CreateLevel<ATitleGameMode, AActor>("TitleLevel");
	UEngineCore::OpenLevel("TitleLevel");
}

void UDoomCore::EngineTick(float DeltaTime)
{
}

void UDoomCore::EngineEnd()
{
}

void UDoomCore::InitWindowSize(UEngineInitData& Data)
{
	float width = GetSystemMetrics(SM_CXSCREEN) - WindowWidth;
	float height = GetSystemMetrics(SM_CYSCREEN) - WindowHeight;

	Data.WindowPosition = { width / 2.0f , height / 2.0f };

	Data.WindowSize = { WindowWidth, WindowHeight };
}
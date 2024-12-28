#include "pch.h"
#include "DoomCore.h"

#include "GameMode/TitleGameMode.h"
#include "GameMode/Round1GameMode.h"
#include "Utils/Wad.h"

#include <Core/Math/EngineMath.h>
#include <Core/EngineCore.h>
#include <Core/Misc/DirectoryHelper.h>
#include <Core/Misc/FileHelper.h>
#include <Classes/Engine/Level.h>
#include <Classes/Engine/Texture.h>
#include <Classes/Engine/PaperSprite.h>
#include <GameFramework/Actor.h>

#include <stdio.h>


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

		DirectoryHelper.Append("Images");

		std::vector<FFileHelper> ImageFiles = DirectoryHelper.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });

		for (int i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UTexture::Load(FilePath);
		}
	}
	{
		FDirectoryHelper DirectoryHelper;
		if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
			return;
		}

		DirectoryHelper.Append("WADs");

		FFileHelper Doom1WadFile = DirectoryHelper.GetFile("doom1.wad");

		UWad* Wad = new UWad();

		LoadWadFromFile(Doom1WadFile.GetPathToString(), Wad);

	}

	UPaperSprite::CreateSpriteToMeta("Player.png", ".sdata");

	UEngineCore::CreateLevel<ATitleGameMode, AActor>("TitleLevel");
	UEngineCore::CreateLevel<ARound1GameMode, AActor>("Round1");
	UEngineCore::OpenLevel("Round1");
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

#define READ_I16(buffer, offset)                                               \
  ((buffer)[(offset)] | ((buffer)[(offset + 1)] << 8))

#define READ_I32(buffer, offset)                                               \
  ((buffer)[(offset)] | ((buffer)[(offset + 1)] << 8) |                        \
  ((buffer)[(offset + 2)] << 16) | ((buffer)[(offset + 3)] << 24))

int UDoomCore::LoadWadFromFile(std::string_view FileName, UWad* Wad)
{
	if (Wad == nullptr)
	{
		MSGASSERT("WAD 주소값이 nullptr 입니다.");
		return 1;
	}
	
	FILE* fp = nullptr; // 포인터 초기화

	errno_t err = fopen_s(&fp, FileName.data(), "rb"); // fopen_s로 파일 열기
	
	if (fp == NULL)
	{
		MSGASSERT("WAD 파일 경로가 잘못되었습니다.");
		return 2;
	}

	fseek(fp, 0, SEEK_END);

	size_t size = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	uint8_t* buffer = static_cast<uint8_t*>(malloc(size));

	if (buffer == NULL) {
		fclose(fp); // 메모리 할당 실패 시 파일 닫기
		return 3;
	}

	fread(buffer, size, 1, fp);
	fclose(fp);

	if(size < 12)
	{
		return 3;
	}
	char* WadID = static_cast<char*>(malloc(5));
	memcpy(WadID, buffer, 4);
	WadID[4] = 0;
	Wad->SetWadID(WadID);
	WadID = nullptr;

	Wad->SetLumps(READ_I32(buffer, 4));
	Wad->SetOffset(READ_I32(buffer, 8));

	free(buffer);
	return 0;
}

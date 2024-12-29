#include "pch.h"
#include "DoomCore.h"

#include "GameMode/TitleGameMode.h"
#include "GameMode/Round1GameMode.h"

#include "Utils/Wad.h"
#include "Utils/Lump.h"
#include "Utils/WadMap.h"

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

		UWadMap* Map = new UWadMap();

		ReadMapFromWad("E1M1", Map, Wad);

		delete Wad;
		Wad = nullptr;
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

	if (size < 12)
	{
		return 3;
	}
	char* WadID = static_cast<char*>(malloc(5));
	memcpy(WadID, buffer, 4);
	WadID[4] = 0;
	Wad->SetWadID(WadID);
	delete WadID;
	WadID = nullptr;

	Wad->SetLumps(READ_I32(buffer, 4));
	uint32_t DirectoryOffset = READ_I32(buffer, 8);

	for (int i = 0; i < Wad->GetLumps(); i++)
	{
		uint32_t Offset = DirectoryOffset + i * 16;

		ULump* Lump = new ULump();

		uint32_t LumpOffset = READ_I32(buffer, Offset);

		Lump->SetSize(READ_I32(buffer, Offset + 4));

		char* LumpName = static_cast<char*>(malloc(9));
		memcpy(LumpName, &buffer[Offset + 8], 8);
		LumpName[8] = 0;
		Lump->SetName(LumpName);

		auto Data = std::make_unique<uint8_t[]>(Lump->GetSize()); 
		std::memcpy(Data.get(), &buffer[LumpOffset], Lump->GetSize()); 
		Lump->SetData(std::move(Data));

		Wad->AddLump(Lump);

		if (i == 10)
		{
			int a = Lump->GetSize();
		}

	}

	free(buffer);
	return 0;
}

int UDoomCore::FindLumpFromWad(std::string_view LumpName, UWad* Wad)
{
	for (int i = 0; i < Wad->GetLumpCount(); i++)
	{
		if (Wad->GetLumpFromVector(i)->GetName() == LumpName)
		{
			return i;
		}
	}
	return 0;
}

#define THINGS_IDX   1
#define LINEDEFS_IDX 2
#define SIDEDEFS_IDX 3
#define VERTEXES_IDX 4
#define SEGS_IDX     5
#define SSECTORS_IDX 6
#define NODES_IDX    7
#define SECTORS_IDX  8

int UDoomCore::ReadMapFromWad(std::string_view MapName, UWadMap* Map, UWad* Wad)
{
	int MapIndex = FindLumpFromWad(MapName, Wad);

	if (MapIndex < 0)
	{
		return 1;
	}

	ReadVertices(Map, Wad->GetLumpFromVector(MapIndex + VERTEXES_IDX));

	return 0;
}

void UDoomCore::ReadVertices(const UWadMap* DoomMap, ULump* Lump)
{
	for (int i = 0, j = 0; i < Lump->GetSize(); i += 4, j++)
	{
		int16_t X = (int16_t)READ_I16(Lump->GetData(), i);
		int16_t Y = (int16_t)READ_I16(Lump->GetData(), i + 2);

		FVector Vertex = {
			static_cast<float>(X),
			static_cast<float>(Y),
			1.0f,
			1.0f
		};
		std::string OutpuString = Vertex.ToString();

	}
}

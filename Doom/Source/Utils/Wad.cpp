#include "pch.h"
#include "Wad.h"

#include "Utils/Wad.h"
#include "Utils/Lump.h"
#include "Utils/WadMap.h"

#include <Core/Misc/DirectoryHelper.h>
#include <Core/Misc/FileHelper.h>

#define THINGS_IDX   1
#define LINEDEFS_IDX 2
#define SIDEDEFS_IDX 3
#define VERTEXES_IDX 4
#define SEGS_IDX     5
#define SSECTORS_IDX 6
#define NODES_IDX    7
#define SECTORS_IDX  8

#define READ_I16(buffer, offset)                                               \
  ((buffer)[(offset)] | ((buffer)[(offset + 1)] << 8))

#define READ_I32(buffer, offset)                                               \
  ((buffer)[(offset)] | ((buffer)[(offset + 1)] << 8) |                        \
  ((buffer)[(offset + 2)] << 16) | ((buffer)[(offset + 3)] << 24))

UWad::UWad()
{
	FDirectoryHelper DirectoryHelper;
	if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
		return;
	}

	DirectoryHelper.Append("WADs");

	FFileHelper Doom1WadFile = DirectoryHelper.GetFile("doom1.wad");

	LoadWadFromFile(Doom1WadFile.GetPathToString());

	Map = new UWadMap();

	ReadMapFromWad("E1M1", Map);
}

UWad::~UWad()
{
	for (int i = 0; i < LumpVector.size(); i++)
	{
		delete LumpVector[i];
		LumpVector[i] = nullptr;
	}

	delete Map;
}



int UWad::LoadWadFromFile(std::string_view FileName)
{
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
	SetWadID(WadID);
	delete WadID;
	WadID = nullptr;

	SetLumps(READ_I32(buffer, 4));
	uint32_t DirectoryOffset = READ_I32(buffer, 8);

	for (int i = 0; i < GetLumps(); i++)
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

		AddLump(Lump);

		if (i == 10)
		{
			int a = Lump->GetSize();
		}

	}

	free(buffer);
	return 0;
}

int UWad::FindLumpFromWad(std::string_view LumpName)
{
	for (int i = 0; i < GetLumpCount(); i++)
	{
		if (GetLumpFromVector(i)->GetName() == LumpName)
		{
			return i;
		}
	}
	return 0;
}

int UWad::ReadMapFromWad(std::string_view MapName, UWadMap* Map)
{
	int MapIndex = FindLumpFromWad(MapName);

	if (MapIndex < 0)
	{
		return 1;
	}

	ReadVertices(Map, GetLumpFromVector(MapIndex + VERTEXES_IDX));
	ReadLines(Map, GetLumpFromVector(MapIndex + LINEDEFS_IDX));

	return 0;
}

void UWad::ReadVertices(UWadMap* DoomMap, ULump* Lump)
{
	DoomMap->SetMinX(INFINITY);
	DoomMap->SetMinY(INFINITY);
	DoomMap->SetMaxX(-INFINITY);
	DoomMap->SetMaxY(-INFINITY);

	for (int i = 0 ; i < Lump->GetSize(); i += 4)
	{
		int16_t X = (int16_t)READ_I16(Lump->GetData(), i);
		int16_t Y = (int16_t)READ_I16(Lump->GetData(), i + 2);

		FVector Vertex = {
			static_cast<float>(X),
			static_cast<float>(Y),
			1.0f,
			1.0f
		};
		
		float MinX = DoomMap->GetMinVector().X;
		float MinY = DoomMap->GetMinVector().Y;
		float MaxX = DoomMap->GetMaxVector().X;
		float MaxY = DoomMap->GetMaxVector().Y;

		if (Vertex.X < DoomMap->GetMinVector().X)
		{
			DoomMap->SetMinX(Vertex.X);
		}
		if (Vertex.Y < DoomMap->GetMinVector().Y)
		{
			DoomMap->SetMinY(Vertex.Y);
		}
		if (Vertex.X > DoomMap->GetMaxVector().X)
		{
			DoomMap->SetMaxX(Vertex.X);
		}
		if (Vertex.Y > DoomMap->GetMaxVector().Y)
		{
			DoomMap->SetMaxY(Vertex.Y);
		}

		DoomMap->AddVertex(Vertex);
	}

	size_t VertexSize = DoomMap->GetVertexCount();
	std::string MaxVectorStr = DoomMap->GetMaxVector().ToString();
	std::string MinVectorStr = DoomMap->GetMinVector().ToString();
}

void UWad::ReadLines(UWadMap* DoomMap, ULump* Lump)
{
	int count = 0;

	int Size = Lump->GetSize();

	for (int i = 0; i < Lump->GetSize(); i += 14)
	{
		int16_t StartIndex = (int16_t)READ_I16(Lump->GetData(), i);
		int16_t EndIndex = (int16_t)READ_I16(Lump->GetData(), i + 2);

		UWadLine* WadLine = new UWadLine();

		WadLine->SetStartIndex(static_cast<float>(StartIndex));
		WadLine->SetEndIndex(static_cast<float>(EndIndex));

		DoomMap->AddWadLine(WadLine);
		count++;
	}
	int a = count;
	DoomMap->GetWadLineByIndex(0);
}

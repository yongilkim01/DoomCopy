#pragma once
#include "Interfaces/IContentsCore.h"

class UWad;
class UWadMap;
class ULump;

/**
 *	����
 */
class UDoomCore : public IContentsCore
{
public:
	/** ������, �Ҹ��� */
	UDoomCore();
	~UDoomCore();

	/** ��ü �� ���� ���� */
	UDoomCore(const UDoomCore& Other) = delete;
	UDoomCore(UDoomCore&& Other) noexcept = delete;
	UDoomCore& operator=(const UDoomCore& Other) = delete;
	UDoomCore& operator=(UDoomCore&& Other) noexcept = delete;

	/** Ŭ���� �޼ҵ� */
	void InitWindowSize(UEngineInitData& Data);
	int LoadWadFromFile(std::string_view FileName, UWad* Wad);
	int FindLumpFromWad(std::string_view LumpName, UWad* Wad);
	int ReadMapFromWad(std::string_view MapName, UWadMap* DoomMap, UWad* Wad);
	void ReadVertices(const UWadMap* DoomMap, ULump* Lump);

	/** ��, �� �޼ҵ� */

protected:
	/** IContentsCore �������̽� �޼ҵ� */
	void EngineStart(UEngineInitData& Data);
	void EngineTick(float DeltaTime);
	void EngineEnd();

private:
	const float WindowWidth = 1280;
	const float WindowHeight = 720;
};
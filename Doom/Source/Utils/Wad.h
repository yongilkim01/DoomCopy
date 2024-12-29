#pragma once

class ULump;

/**
 *	Wad File class
 */
class UWad
{
public:
	/** ������, �Ҹ��� */
	UWad();
	~UWad();

	/** ��ü �� ������� */
	UWad(const UWad& Other) = delete;
	UWad(UWad&& Other) noexcept = delete;
	UWad& operator=(const UWad& Other) = delete;
	UWad& operator=(UWad&& Other) noexcept = delete;

	/** ��, �� �޼ҵ� */
	void SetWadID(std::string_view NewWadID)
	{
		WadID = NewWadID;
	}
	std::string_view GetWadID()
	{
		return WadID;
	}
	void SetLumps(uint32_t Lumps)
	{
		nLumps = Lumps;
	}
	uint32_t GetLumps()
	{
		return nLumps;
	}
	void AddLump(ULump* Lump)
	{
		LumpVector.push_back(Lump);
	}
	size_t GetLumpCount()
	{
		return LumpVector.size();
	}
	ULump* GetLumpFromVector(int Index)
	{
		return LumpVector[Index];
	}
protected:


private:
	std::string WadID;
	uint32_t nLumps = 0;

	std::vector<ULump*> LumpVector;
};


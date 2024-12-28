#pragma once

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
	void SetOffset(uint32_t NewOffset)
	{
		Offset = NewOffset;
	}
	uint32_t GetOffset()
	{
		return Offset;
	}
protected:


private:
	std::string WadID;
	uint32_t nLumps = 0;
	uint32_t Offset = 0;

};


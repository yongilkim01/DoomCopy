#pragma once

/**
 *	Lump File class
 */
class ULump
{
public:
	/** ������, �Ҹ��� */
	ULump();
	~ULump();

	/** ��ü �� ������� */
	ULump(const ULump& Other) = delete;
	ULump(ULump&& Other) noexcept = delete;
	ULump& operator=(const ULump& Other) = delete;
	ULump& operator=(ULump&& Other) noexcept = delete;

	/** ��, �� �޼ҵ� */
	void SetName(std::string_view NewName)
	{
		Name = NewName;
	}
	std::string_view GetName()
	{
		return Name;
	}
	void SetSize(uint32_t NewSize)
	{
		Size = NewSize;
	}
	uint32_t GetSize()
	{
		return Size;
	}
	void SetData(std::unique_ptr<uint8_t[]> NewData) 
	{
		this->Data = std::move(NewData);
	}
	const uint8_t* GetData() const
	{
		return Data.get();
	}
protected:


private:
	std::string Name;
	
	std::unique_ptr<uint8_t[]> Data;

	uint32_t Size = 0;

};


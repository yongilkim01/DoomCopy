#pragma once
#include <vector>
#include <string>

#include "Core/Public/Math/EngineMath.h"

class IArchiveObject;

/**
 *	파일 직렬화 저장 클래스
 */
class FArchive
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API FArchive();
	ENGINE_API ~FArchive();

	/** 객체 복사 방지 */
	//FArchive(const FArchive& _Other) = delete;
	//FArchive(FArchive&& _Other) noexcept = delete;
	//FArchive& operator=(const FArchive& _Other) = delete;
	//FArchive& operator=(FArchive&& _Other) noexcept = delete;

	/** 직렬화 메소드 */
	ENGINE_API void Write(const void* WriteData, unsigned int Size);
	ENGINE_API void Read(void* ReadData, unsigned int Size);

	/** 연산자 */
	void operator<<(const std::string& WriteData)
	{
		int Size = static_cast<int>(WriteData.size());
		operator<<(Size);
		if (0 != Size)
		{
			Write(&WriteData[0], static_cast<int>(WriteData.size()));
		}
	}
	template<typename DataType>
	void operator<<(std::vector<DataType>& WriteVector)
	{
		int Size = static_cast<int>(WriteVector.size());
		operator<<(Size);
		for (size_t i = 0; i < WriteVector.size(); i++)
		{
			operator<<(WriteVector[i]);
		}
	}
	void operator>>(std::string& ReadData)
	{
		int Size;
		operator>>(Size);
		ReadData.resize(Size);
		Read(&ReadData[0], static_cast<int>(ReadData.size()));
	}
	template<typename DataType>
	void operator>>(std::vector<DataType>& ReadVector)
	{
		int Size = 0;
		operator>>(Size);
		ReadVector.resize(Size);

		for (size_t i = 0; i < ReadVector.size(); i++)
		{
			operator>>(ReadVector[i]);
		}
	}
	void operator<<(IArchiveObject& Ser);
	void operator>>(IArchiveObject& Ser);
	void operator<<(const int& WriteData)
	{
		Write(&WriteData, sizeof(int));
	}
	void operator<<(const bool& WriteData)
	{
		Write(&WriteData, sizeof(bool));
	}
	void operator<<(const FVector& WriteData)
	{
		Write(&WriteData, sizeof(FVector));
	}
	void operator<<(const FIntPoint& WriteData)
	{
		Write(&WriteData, sizeof(FIntPoint));
	}
	void operator>>(int& ReadData)
	{
		Read(&ReadData, sizeof(int));
	}
	void operator>>(bool& ReadData)
	{
		Read(&ReadData, sizeof(bool));
	}
	void operator>>(FVector& ReadData)
	{
		Read(&ReadData, sizeof(FVector));
	}
	void operator>>(FIntPoint& ReadData)
	{
		Read(&ReadData, sizeof(FIntPoint));
	}

	/** 겟, 셋 메소드 */
	void* GetDataPtr()
	{
		return &Data[0];
	}
	int GetWriteOffset()
	{
		return WriteOffset;
	}
	void DataResize(int Value)
	{
		Data.resize(Value);
	}

protected:

private:
	/** 바이트 패딩 */
	int WriteOffset = 0;
	int ReadOffset = 0;

	std::vector<char> Data;
};

class IArchiveObject
{
public:
	ENGINE_API virtual ~IArchiveObject() = 0
	{

	}

public:
	ENGINE_API virtual void Serialize(FArchive& Ser) = 0;
	ENGINE_API virtual void DeSerialize(FArchive& Ser) = 0;
};
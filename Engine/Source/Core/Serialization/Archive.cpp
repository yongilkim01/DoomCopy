#include "pch.h"
#include "Archive.h"

void IArchiveObject::Serialize(FArchive& Ser)
{

}

void IArchiveObject::DeSerialize(FArchive& Ser)
{

}

FArchive::FArchive()
{
}

FArchive::~FArchive()
{
}

void FArchive::Write(const void* WriteData, unsigned int Size)
{
	if (WriteOffset + Size >= Data.size())
	{
		Data.resize(Data.size() * 2 + Size);
	}

	memcpy_s(&Data[WriteOffset], Size, WriteData, Size);
	WriteOffset += Size;
}

void FArchive::operator<<(IArchiveObject& Ser)
{
	Ser.Serialize(*this);
}

void FArchive::Read(void* ReadData, unsigned int Size)
{
	memcpy_s(ReadData, Size, &Data[ReadOffset], Size);
	ReadOffset += Size;
}

void FArchive::operator>>(IArchiveObject& Ser)
{
	Ser.DeSerialize(*this);
}

#include "pch.h"
#include "FileHelper.h"
#include "WorldPartition/DebugHelpers.h"
#include "Core/Serialization/Archive.h"

FFileHelper::FFileHelper()
{


}

FFileHelper::FFileHelper(const std::string& Path)
	: FPaths(std::string_view(Path.c_str()))
{
}

FFileHelper::FFileHelper(std::string_view Path)
	: FPaths(Path)
{

}

FFileHelper::FFileHelper(std::filesystem::path Path)
	: FPaths(Path)
{

}

FFileHelper::~FFileHelper()
{
	Close();
}

void FFileHelper::FileOpen(const char* Mode)
{
	fopen_s(&File, GetPathToString().c_str(), Mode);

	if (nullptr == File)
	{
		MSGASSERT(GetPathToString() + +"���� ���¿� �����߽��ϴ�");
	}
}


void FFileHelper::Write(const void* Ptr, size_t Size)
{
	if (0 == Size)
	{
		MSGASSERT("ũ�Ⱑ 0�� �����͸� ������ �����ϴ�.");
	}

	if (nullptr == Ptr)
	{
		MSGASSERT("�������� �ʴ� �޸𸮸� ����Ϸ��� �߽��ϴ�.");
	}

	// w�� ��쿡 ���� ����ó��
	if (nullptr == File)
	{
		MSGASSERT("���� ���� ���Ͽ� ������ ������ �߽��ϴ�");
		return;
	}

	fwrite(Ptr, Size, 1, File);
}

void FFileHelper::Write(FArchive& Ser)
{
	Write(Ser.GetDataPtr(), Ser.GetWriteOffset());
}

void FFileHelper::Read(void* Ptr, size_t Size)
{
	if (0 == Size)
	{
		MSGASSERT("ũ�Ⱑ 0�� �����͸� �������� �����ϴ�.");
	}

	if (nullptr == Ptr)
	{
		MSGASSERT("�������� �ʴ� �޸𸮸� �������� �߽��ϴ�.");
	}

	if (nullptr == File)
	{
		MSGASSERT("���� ���� ���Ͽ� ������ ������ �߽��ϴ�");
		return;
	}

	fread(Ptr, Size, 1, File);
}

void FFileHelper::Read(FArchive& Ser)
{
	int FileSize = GetFileSize();

	Ser.DataResize(FileSize);

	Read(Ser.GetDataPtr(), FileSize);
}

void FFileHelper::Close()
{
	if (nullptr != File)
	{
		fclose(File);
		File = nullptr;
	}
}

int FFileHelper::GetFileSize()
{
	if (false == IsFile())
	{
		MSGASSERT(Path.string() + "������ �ƴ� ������ ũ�⸦ �˼��� �����ϴ�.");
		return -1;
	}

	return static_cast<int>(std::filesystem::file_size(Path));
}


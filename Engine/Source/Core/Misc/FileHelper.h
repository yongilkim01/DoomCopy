#pragma once
#include <Windows.h>
#include <iostream>
#include <io.h>

#include "Paths.h"

class FArchive;

const int MAXPATH = 256;

class FFileHelper : public FPaths
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API FFileHelper();
	ENGINE_API FFileHelper(const std::string& Path);
	ENGINE_API FFileHelper(std::string_view Path);
	ENGINE_API FFileHelper(std::filesystem::path Path);
	ENGINE_API FFileHelper(const FPaths& PathData);
	ENGINE_API ~FFileHelper();

	/** ���� ���� �б� �޼ҵ� */
	void Write(const void* Ptr, size_t Size);
	void Write(FArchive& Ser);
	void Read(void* Ptr, size_t Size);
	void Read(FArchive& Ser);

	/** ���� ���� �ݱ� �޼ҵ� */
	ENGINE_API void FileOpen(const char* Mode);
	void Close();
	int GetFileSize();

	ENGINE_API std::string GetAllFileText();

private:
	FILE* File = nullptr;
};


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
	FFileHelper();
	FFileHelper(const std::string& Path);
	FFileHelper(std::string_view Path);
	FFileHelper(std::filesystem::path Path);
	FFileHelper(const FPaths& PathData);
	~FFileHelper();

	/** ���� ���� �б� �޼ҵ� */
	void Write(const void* Ptr, size_t Size);
	void Write(FArchive& Ser);
	void Read(void* Ptr, size_t Size);
	void Read(FArchive& Ser);

	/** ���� ���� �ݱ� �޼ҵ� */
	ENGINE_API void FileOpen(const char* Mode);
	void Close();
	int GetFileSize();

	std::string GetAllFileText();

private:
	FILE* File = nullptr;
};


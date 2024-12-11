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
	/** 생성자, 소멸자 */
	FFileHelper();
	FFileHelper(const std::string& Path);
	FFileHelper(std::string_view Path);
	FFileHelper(std::filesystem::path Path);
	~FFileHelper();

	/** 파일 쓰기 읽기 메소드 */
	void Write(const void* Ptr, size_t Size);
	void Write(FArchive& Ser);
	void Read(void* Ptr, size_t Size);
	void Read(FArchive& Ser);

	/** 파일 열고 닫기 메소드 */
	void FileOpen(const char* Mode);
	void Close();
	int GetFileSize();

private:
	FILE* File = nullptr;
};


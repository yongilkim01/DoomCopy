#include "pch.h"
#include "DirectoryHelper.h"
#include "FileHelper.h"

#include "WorldPartition/DebugHelpers.h"

FDirectoryHelper::FDirectoryHelper()
	: FPaths()
{

}

FDirectoryHelper::FDirectoryHelper(std::string_view InitPath)
	: FPaths(InitPath)
{
}

FDirectoryHelper::FDirectoryHelper(std::filesystem::path InitPath)
	: FPaths(InitPath)
{
}

FDirectoryHelper::~FDirectoryHelper()
{

}

std::vector<class FFileHelper> FDirectoryHelper::GetAllFile(bool IsRecursive /* = true */)
{
	// 결과를 반환할 벡터 객체
	std::vector<class FFileHelper> Result;

	// 디렉토리 내 항목을 순회하는 이터레이터 타입 변수를 통해서 지정된 디렉토리 내에 어떤 파일 들이 있는지를 검사
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	// 디렉토리 내 항목을 순회
	while (Diriter._At_end() == false)
	{
		// 현재 경로를 새로운 변수에 할당
		std::filesystem::path FilePath = *Diriter;

		// 파일 경로를 통해서 FPaths 객체 생성
		FPaths Path = FPaths(FilePath);

		// 파일 경로의 디렉토리가 유효할 경우
		if (Path.IsDirectory() == true)
		{
			// IsRecursive가 true일 경우
			if (IsRecursive == true)
			{
				GetAllFileRecursive(FilePath, Result);
			}

			++Diriter;
			continue;
		}

		// 결과 벡터에 추가
		Result.push_back(FFileHelper(FilePath));
		// 순환을 위해서 이터레이터 추가
		++Diriter;
	}
	// 결과 벡터 반환
	return Result;
}

std::vector<class FDirectoryHelper> FDirectoryHelper::GetAllDirectory()
{
	// 결과를 반환할 벡터 객체
	std::vector<class FDirectoryHelper> Result;

	// 경로를 넣어주면 그 경로의 첫번째 파일을 가리키게 된다.
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	while (Diriter._At_end() == false)
	{
		std::filesystem::path FilePath = *Diriter;

		FPaths Path = FPaths(FilePath);

		if (Path.IsDirectory() == false)
		{
			++Diriter;
			continue;
		}

		Result.push_back(FDirectoryHelper(FilePath));
		++Diriter;
	}

	return Result;
}

ENGINE_API FFileHelper FDirectoryHelper::GetFile(std::string_view FileName)
{
	FPaths FilePath = Path;
	FilePath.Append(FileName);

	if (false == FilePath.IsExists())
	{
		MSGASSERT("존재하지 않는 파일을 객체화하려고 했습니다.");
	}

	return FFileHelper(FilePath);
}

void FDirectoryHelper::GetAllFileRecursive(std::filesystem::path Path, std::vector<class FFileHelper>& Result)
{
	// Path의 첫번째 파일 또는 디렉토리를 포커스
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	// 순회를 위한 반복문
	while (Diriter._At_end() == false)
	{
		// 현재 파일 경로를 변수에 할당
		std::filesystem::path FilePath = *Diriter;

		// 파일 경로를 통해서 FPaths 객체 생성
		FPaths Path = FPaths(FilePath);

		// 파일 경로의 디렉토리가 유효할 경우
		if (Path.IsDirectory() == true)
		{
			GetAllFileRecursive(FilePath, Result);
			++Diriter;
			continue;
		}

		// 결과 배열의 추가
		Result.push_back(FFileHelper(FilePath));
		++Diriter;
	}
}
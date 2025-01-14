#include "pch.h"
#include "Core/Public/Misc/DirectoryHelper.h"
#include "Core/Public/Misc/FileHelper.h"

#include "Core/Public/Debugging/DebugMacros.h"
#include "Core/Public/Containers/EngineString.h"

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

//std::vector<class FFileHelper> FDirectoryHelper::GetAllFile(bool IsRecursive /* = true */)
//{
//	// 결과를 반환할 벡터 객체
//	std::vector<class FFileHelper> Result;
//
//	// 디렉토리 내 항목을 순회하는 이터레이터 타입 변수를 통해서 지정된 디렉토리 내에 어떤 파일 들이 있는지를 검사
//	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);
//
//	// 디렉토리 내 항목을 순회
//	while (Diriter._At_end() == false)
//	{
//		// 현재 경로를 새로운 변수에 할당
//		std::filesystem::path FilePath = *Diriter;
//
//		// 파일 경로를 통해서 FPaths 객체 생성
//		FPaths Path = FPaths(FilePath);
//
//		// 파일 경로의 디렉토리가 유효할 경우
//		if (Path.IsDirectory() == true)
//		{
//			// IsRecursive가 true일 경우
//			if (IsRecursive == true)
//			{
//				GetAllFileRecursive(FilePath, Result, Exts);
//			}
//
//			++Diriter;
//			continue;
//		}
//
//		// 결과 벡터에 추가
//		Result.push_back(FFileHelper(FilePath));
//		// 순환을 위해서 이터레이터 추가
//		++Diriter;
//	}
//	// 결과 벡터 반환
//	return Result;
//}

std::vector<class FFileHelper> FDirectoryHelper::GetAllFile(bool IsRecursive, const std::vector<std::string>& Exts)
{
	std::vector<std::string> UpperExtensionVector;

	for (size_t i = 0; i < Exts.size(); i++)
	{
		UpperExtensionVector.push_back(UEngineString::ToUpper(Exts[i]));
	}

	// 결과를 반환할 벡터 객체
	std::vector<FFileHelper> Result;

	// 디렉토리 내 항목을 순회하는 이터레이터 타입 변수를 통해서 지정된 디렉토리 내에 어떤 파일 들이 있는지를 검사
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	// 디렉토리 이터레이터가 끝에 도달할 때까지 반복
	while (false == Diriter._At_end())
	{
		// 현재 파일 경로를 가져옴
		std::filesystem::path FilePath = *Diriter;

		// 파일 경로를 FPaths 객체로 변환
		FPaths Path = FPaths(FilePath);

		// 현재 경로가 디렉토리인지 확인
		if (true == Path.IsDirectory())
		{
			// 재귀적으로 파일을 검색할 경우
			if (true == IsRecursive)
			{
				// 재귀적으로 파일을 검색
				GetAllFileRecursive(FilePath, Result, Exts);
			}

			// 다음 이터레이터로 이동
			++Diriter;
			continue;
		}

		// 파일 확장자를 검사하기 위한 플래그 초기화
		bool Check = true;

		// 허용된 확장자 목록과 현재 파일의 확장자를 비교
		for (size_t i = 0; i < UpperExtensionVector.size(); i++)
		{
			// 현재 파일의 확장자를 대문자로 변환
			std::string CurUpperExt = UEngineString::ToUpper(Path.GetExtension());

			// 현재 파일의 확장자가 허용된 확장자 목록에 있는지 확인
			if (CurUpperExt == UpperExtensionVector[i])
			{
				Check = false; // 허용된 확장자이면 Check를 false로 설정
				break;
			}
		}

		// 허용되지 않은 확장자인 경우 다음 파일로 이동
		if (true == Check)
		{
			++Diriter;
			continue;
		}

		// 결과 벡터에 파일을 추가
		Result.push_back(FFileHelper(FilePath));
		// 다음 이터레이터로 이동
		++Diriter;
	}

	// 결과 반환
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

	//if (false == FilePath.IsExists())
	//{
	//	MSGASSERT("존재하지 않는 파일을 객체화하려고 했습니다.");
	//}

	return FFileHelper(FilePath);
}

void FDirectoryHelper::GetAllFileRecursive(std::filesystem::path Path, std::vector<class FFileHelper>& Result, const std::vector<std::string>& Exts)
{
	std::vector<std::string> UpperExts;

	for (size_t i = 0; i < Exts.size(); i++)
	{
		UpperExts.push_back(UEngineString::ToUpper(Exts[i]));
	}

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
			GetAllFileRecursive(FilePath, Result, Exts);
			++Diriter;
			continue;
		}

		bool Check = true;

		for (size_t i = 0; i < UpperExts.size(); i++)
		{
			std::string CurUpperExt = UEngineString::ToUpper(Path.GetExtension());
			if (CurUpperExt == UpperExts[i])
			{
				Check = false;
				break;
			}
		}

		if (true == Check)
		{
			++Diriter;
			continue;
		}

		// 결과 배열의 추가
		Result.push_back(FFileHelper(FilePath));
		++Diriter;
	}
}
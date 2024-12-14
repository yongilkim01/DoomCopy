#pragma once
#include "Paths.h"

class FFileHelper;

/**
 * 디렉토리 파일 순회 클래스
 */
class FDirectoryHelper : public FPaths
{
public:
	/** 생성자, 소멸자 */
	FDirectoryHelper();
	FDirectoryHelper(std::string_view InitPath);
	FDirectoryHelper(std::filesystem::path InitPath);
	~FDirectoryHelper();

	/**
	 * 디렉토리 내 모든 파일들을 가져와서 FFileHelper 타입으로 값을 저장 후 반환하는 메소드

	 * @param IsRecursive - 재귀를 통한 디렉토리 순회 여부
	 */
	std::vector<class FFileHelper> GetAllFile(bool IsRecursive = true);

	/** 디렉토리 내 모든 디렉토리들을 가져와서 UEngineDirectory 타입으로 값을 저장 후 반환하는 메소드 */
	std::vector<class FDirectoryHelper> GetAllDirectory();

	ENGINE_API FFileHelper GetFile(std::string_view FileName);

protected:

private:
	/**
	 * 디렉토리 내 모든 파일들을 재귀 형식으로 가져오는 메소드, UEngineDirectory::GetAllFile에서 IsRecursive를 true로 설정하였을 때 호출
	 *
	 * @param Path - 디렉토리 경로
	 * @param Result - 결과 값을 저장할 벡터의 참조 객체
	 */
	void GetAllFileRecursive(std::filesystem::path Path, std::vector<class FFileHelper>& Result);
};


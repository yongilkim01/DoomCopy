#pragma once
#include <filesystem>

/**
 * 파일 경로를 객체화한 클래스
 */
class FPaths
{
public:
	/** 생성자, 소멸자 */
	FPaths();
	FPaths(std::string_view _Path);
	FPaths(std::filesystem::path _Path);
	~FPaths();

	/** 전체 파일 경로 */
	std::string GetPathToString();
	/** 파일명 + 확장자 */
	std::string GetFileName();
	/** 디렉토리명 + 확장자 */
	std::string GetDirectoryName();
	/** 확장자 */
	ENGINE_API std::string GetExtension();

	void Append(std::string_view _AppendName);
	void MoveParent();
	bool MoveParentToDirectory(std::string_view _Path);
	bool Move(std::string_view Path);

	bool IsDirectory();
	bool IsFile();
	bool IsExists();

protected:
	std::filesystem::path Path;

private:

};
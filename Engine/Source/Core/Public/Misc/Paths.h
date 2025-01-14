#pragma once
#include <filesystem>

#include "EngineDefine.h"

/**
 * ���� ��θ� ��üȭ�� Ŭ����
 */
class FPaths
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API FPaths();
	ENGINE_API FPaths(std::string_view _Path);
	ENGINE_API FPaths(std::filesystem::path _Path);
	ENGINE_API ~FPaths();

	/** ��ü ���� ��� */
	ENGINE_API std::string GetPathToString();
	/** ���ϸ� + Ȯ���� */
	ENGINE_API std::string GetFileName();
	/** ���丮�� + Ȯ���� */
	ENGINE_API std::string GetDirectoryName();
	/** Ȯ���� */
	ENGINE_API std::string GetExtension();

	ENGINE_API void Append(std::string_view _AppendName);
	ENGINE_API void MoveParent();
	ENGINE_API bool MoveParentToDirectory(std::string_view _Path);
	ENGINE_API bool MoveEngineShaderDirectory();
	ENGINE_API bool MoveEngineDirectory();
	ENGINE_API bool MoveSelectDirectory(std::string_view DirectoryName);
	ENGINE_API bool MoveSelectShaderDirectory(std::string_view DirectoryName);
	ENGINE_API bool Move(std::string_view Path);

	ENGINE_API bool IsDirectory();
	ENGINE_API bool IsFile();
	ENGINE_API bool IsExists();

protected:
	std::filesystem::path Path;

private:

};
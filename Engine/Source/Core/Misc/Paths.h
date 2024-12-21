#pragma once
#include <filesystem>

/**
 * ���� ��θ� ��üȭ�� Ŭ����
 */
class FPaths
{
public:
	/** ������, �Ҹ��� */
	FPaths();
	FPaths(std::string_view _Path);
	FPaths(std::filesystem::path _Path);
	~FPaths();

	/** ��ü ���� ��� */
	std::string GetPathToString();
	/** ���ϸ� + Ȯ���� */
	std::string GetFileName();
	/** ���丮�� + Ȯ���� */
	std::string GetDirectoryName();
	/** Ȯ���� */
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
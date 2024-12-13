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
	// ����� ��ȯ�� ���� ��ü
	std::vector<class FFileHelper> Result;

	// ���丮 �� �׸��� ��ȸ�ϴ� ���ͷ����� Ÿ�� ������ ���ؼ� ������ ���丮 ���� � ���� ���� �ִ����� �˻�
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	// ���丮 �� �׸��� ��ȸ
	while (Diriter._At_end() == false)
	{
		// ���� ��θ� ���ο� ������ �Ҵ�
		std::filesystem::path FilePath = *Diriter;

		// ���� ��θ� ���ؼ� FPaths ��ü ����
		FPaths Path = FPaths(FilePath);

		// ���� ����� ���丮�� ��ȿ�� ���
		if (Path.IsDirectory() == true)
		{
			// IsRecursive�� true�� ���
			if (IsRecursive == true)
			{
				GetAllFileRecursive(FilePath, Result);
			}

			++Diriter;
			continue;
		}

		// ��� ���Ϳ� �߰�
		Result.push_back(FFileHelper(FilePath));
		// ��ȯ�� ���ؼ� ���ͷ����� �߰�
		++Diriter;
	}
	// ��� ���� ��ȯ
	return Result;
}

std::vector<class FDirectoryHelper> FDirectoryHelper::GetAllDirectory()
{
	// ����� ��ȯ�� ���� ��ü
	std::vector<class FDirectoryHelper> Result;

	// ��θ� �־��ָ� �� ����� ù��° ������ ����Ű�� �ȴ�.
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
		MSGASSERT("�������� �ʴ� ������ ��üȭ�Ϸ��� �߽��ϴ�.");
	}

	return FFileHelper(FilePath);
}

void FDirectoryHelper::GetAllFileRecursive(std::filesystem::path Path, std::vector<class FFileHelper>& Result)
{
	// Path�� ù��° ���� �Ǵ� ���丮�� ��Ŀ��
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	// ��ȸ�� ���� �ݺ���
	while (Diriter._At_end() == false)
	{
		// ���� ���� ��θ� ������ �Ҵ�
		std::filesystem::path FilePath = *Diriter;

		// ���� ��θ� ���ؼ� FPaths ��ü ����
		FPaths Path = FPaths(FilePath);

		// ���� ����� ���丮�� ��ȿ�� ���
		if (Path.IsDirectory() == true)
		{
			GetAllFileRecursive(FilePath, Result);
			++Diriter;
			continue;
		}

		// ��� �迭�� �߰�
		Result.push_back(FFileHelper(FilePath));
		++Diriter;
	}
}
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
//	// ����� ��ȯ�� ���� ��ü
//	std::vector<class FFileHelper> Result;
//
//	// ���丮 �� �׸��� ��ȸ�ϴ� ���ͷ����� Ÿ�� ������ ���ؼ� ������ ���丮 ���� � ���� ���� �ִ����� �˻�
//	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);
//
//	// ���丮 �� �׸��� ��ȸ
//	while (Diriter._At_end() == false)
//	{
//		// ���� ��θ� ���ο� ������ �Ҵ�
//		std::filesystem::path FilePath = *Diriter;
//
//		// ���� ��θ� ���ؼ� FPaths ��ü ����
//		FPaths Path = FPaths(FilePath);
//
//		// ���� ����� ���丮�� ��ȿ�� ���
//		if (Path.IsDirectory() == true)
//		{
//			// IsRecursive�� true�� ���
//			if (IsRecursive == true)
//			{
//				GetAllFileRecursive(FilePath, Result, Exts);
//			}
//
//			++Diriter;
//			continue;
//		}
//
//		// ��� ���Ϳ� �߰�
//		Result.push_back(FFileHelper(FilePath));
//		// ��ȯ�� ���ؼ� ���ͷ����� �߰�
//		++Diriter;
//	}
//	// ��� ���� ��ȯ
//	return Result;
//}

std::vector<class FFileHelper> FDirectoryHelper::GetAllFile(bool IsRecursive, const std::vector<std::string>& Exts)
{
	std::vector<std::string> UpperExtensionVector;

	for (size_t i = 0; i < Exts.size(); i++)
	{
		UpperExtensionVector.push_back(UEngineString::ToUpper(Exts[i]));
	}

	// ����� ��ȯ�� ���� ��ü
	std::vector<FFileHelper> Result;

	// ���丮 �� �׸��� ��ȸ�ϴ� ���ͷ����� Ÿ�� ������ ���ؼ� ������ ���丮 ���� � ���� ���� �ִ����� �˻�
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	// ���丮 ���ͷ����Ͱ� ���� ������ ������ �ݺ�
	while (false == Diriter._At_end())
	{
		// ���� ���� ��θ� ������
		std::filesystem::path FilePath = *Diriter;

		// ���� ��θ� FPaths ��ü�� ��ȯ
		FPaths Path = FPaths(FilePath);

		// ���� ��ΰ� ���丮���� Ȯ��
		if (true == Path.IsDirectory())
		{
			// ��������� ������ �˻��� ���
			if (true == IsRecursive)
			{
				// ��������� ������ �˻�
				GetAllFileRecursive(FilePath, Result, Exts);
			}

			// ���� ���ͷ����ͷ� �̵�
			++Diriter;
			continue;
		}

		// ���� Ȯ���ڸ� �˻��ϱ� ���� �÷��� �ʱ�ȭ
		bool Check = true;

		// ���� Ȯ���� ��ϰ� ���� ������ Ȯ���ڸ� ��
		for (size_t i = 0; i < UpperExtensionVector.size(); i++)
		{
			// ���� ������ Ȯ���ڸ� �빮�ڷ� ��ȯ
			std::string CurUpperExt = UEngineString::ToUpper(Path.GetExtension());

			// ���� ������ Ȯ���ڰ� ���� Ȯ���� ��Ͽ� �ִ��� Ȯ��
			if (CurUpperExt == UpperExtensionVector[i])
			{
				Check = false; // ���� Ȯ�����̸� Check�� false�� ����
				break;
			}
		}

		// ������ ���� Ȯ������ ��� ���� ���Ϸ� �̵�
		if (true == Check)
		{
			++Diriter;
			continue;
		}

		// ��� ���Ϳ� ������ �߰�
		Result.push_back(FFileHelper(FilePath));
		// ���� ���ͷ����ͷ� �̵�
		++Diriter;
	}

	// ��� ��ȯ
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

	//if (false == FilePath.IsExists())
	//{
	//	MSGASSERT("�������� �ʴ� ������ ��üȭ�Ϸ��� �߽��ϴ�.");
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

		// ��� �迭�� �߰�
		Result.push_back(FFileHelper(FilePath));
		++Diriter;
	}
}
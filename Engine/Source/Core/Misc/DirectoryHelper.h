#pragma once
#include "Paths.h"

class FFileHelper;

/**
 * ���丮 ���� ��ȸ Ŭ����
 */
class FDirectoryHelper : public FPaths
{
public:
	/** ������, �Ҹ��� */
	FDirectoryHelper();
	FDirectoryHelper(std::string_view InitPath);
	FDirectoryHelper(std::filesystem::path InitPath);
	~FDirectoryHelper();

	/**
	 * ���丮 �� ��� ���ϵ��� �����ͼ� FFileHelper Ÿ������ ���� ���� �� ��ȯ�ϴ� �޼ҵ�

	 * @param IsRecursive - ��͸� ���� ���丮 ��ȸ ����
	 */
	std::vector<class FFileHelper> GetAllFile(bool IsRecursive = true);

	/** ���丮 �� ��� ���丮���� �����ͼ� UEngineDirectory Ÿ������ ���� ���� �� ��ȯ�ϴ� �޼ҵ� */
	std::vector<class FDirectoryHelper> GetAllDirectory();

	ENGINE_API FFileHelper GetFile(std::string_view FileName);

protected:

private:
	/**
	 * ���丮 �� ��� ���ϵ��� ��� �������� �������� �޼ҵ�, UEngineDirectory::GetAllFile���� IsRecursive�� true�� �����Ͽ��� �� ȣ��
	 *
	 * @param Path - ���丮 ���
	 * @param Result - ��� ���� ������ ������ ���� ��ü
	 */
	void GetAllFileRecursive(std::filesystem::path Path, std::vector<class FFileHelper>& Result);
};


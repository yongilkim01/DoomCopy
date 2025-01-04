#pragma once
#include "Core/Object/Object.h"

class UAssetImportTask : public UObject
{
public:
	ENGINE_API UAssetImportTask() {}
	ENGINE_API ~UAssetImportTask() {}

	std::string FileName = "";
	std::string DestinationPath = "";
	bool bAutomated = false;
};


/**
 *	����
 */
class UAssetLibrary
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UAssetLibrary();
	ENGINE_API ~UAssetLibrary();

	/** ��ü �� ���� ���� */
	UAssetLibrary(const UAssetLibrary& Other) = delete;
	UAssetLibrary(UAssetLibrary&& Other) noexcept = delete;
	UAssetLibrary& operator=(const UAssetLibrary& Other) = delete;
	UAssetLibrary& operator=(UAssetLibrary&& Other) noexcept = delete;

	ENGINE_API static void ImportAsset(UAssetImportTask* ImportTask);


protected:

private:

};


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
 *	설명
 */
class UAssetLibrary
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UAssetLibrary();
	ENGINE_API ~UAssetLibrary();

	/** 객체 값 복사 방지 */
	UAssetLibrary(const UAssetLibrary& Other) = delete;
	UAssetLibrary(UAssetLibrary&& Other) noexcept = delete;
	UAssetLibrary& operator=(const UAssetLibrary& Other) = delete;
	UAssetLibrary& operator=(UAssetLibrary&& Other) noexcept = delete;

	ENGINE_API static void ImportAsset(UAssetImportTask* ImportTask);


protected:

private:

};


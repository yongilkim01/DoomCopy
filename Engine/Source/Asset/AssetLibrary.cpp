#include "pch.h"
#include "AssetLibrary.h"

#include "Core/Misc/Paths.h"
#include "Core/Misc/FileHelper.h"

UAssetLibrary::UAssetLibrary()
{
}

UAssetLibrary::~UAssetLibrary()
{
}

void UAssetLibrary::ImportAsset(UAssetImportTask* ImportTask)
{
	std::string_view PathStr = ImportTask->FileName;
	
	FFileHelper FileHelper = FPaths(PathStr);

	std::string Extension = FileHelper.GetExtension();

	if (Extension == "obj")
	{

	}
}

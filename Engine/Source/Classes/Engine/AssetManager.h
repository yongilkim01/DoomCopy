#pragma once
#include "Core/Object/Object.h"
#include "Core/Misc/Paths.h"

/**
 *	����
 */
template<typename AssetType>
class UAssetManager  : public UObject
{
public:
	/** ������, �Ҹ��� */
	UAssetManager() {}
	~UAssetManager() {}

	/** ��ü �� ���� ���� */
	UAssetManager(const UAssetManager& Other) = delete;
	UAssetManager(UAssetManager&& Other) noexcept = delete;
	UAssetManager& operator=(const UAssetManager& Other) = delete;
	UAssetManager& operator=(UAssetManager&& Other) noexcept = delete;

	static std::shared_ptr<AssetType> Find(std::string_view FindResourceName)
	{
		return AssetMap[FindResourceName];
	}

protected:

private:
	FPaths Path;

	static std::map<std::string, std::shared_ptr<AssetType>> AssetMap;
};

template<typename AssetType>
std::map<std::string, std::shared_ptr<AssetType>> UAssetManager<AssetType>::AssetMap;

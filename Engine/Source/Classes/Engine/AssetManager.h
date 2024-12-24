#pragma once
#include "Core/Object/Object.h"
#include "Core/Misc/Paths.h"
#include "Core/Containers/EngineString.h"

/**
 *	설명
 */
class UAssetManager  : public UObject
{
public:
	/** 생성자, 소멸자 */
	UAssetManager() {}
	~UAssetManager() {}

	/** 객체 값 복사 방지 */
	UAssetManager(const UAssetManager& Other) = delete;
	UAssetManager(UAssetManager&& Other) noexcept = delete;
	UAssetManager& operator=(const UAssetManager& Other) = delete;
	UAssetManager& operator=(UAssetManager&& Other) noexcept = delete;

	static std::string ToUpperName(std::string_view Name)
	{
		return UEngineString::ToUpper(Name);
	}

	template<typename AssetType>
	static std::shared_ptr<AssetType> Find(std::string_view AssetName)
	{
		const type_info& Info = typeid(AssetType);
		return std::dynamic_pointer_cast<AssetType>(Find(Info.name(), AssetName.data()));
	}
	static std::shared_ptr<UAssetManager> Find(std::string_view AssetName, std::string_view Name);

	static bool Contains(std::string_view AssetName)
	{
		return AssetMap.contains(AssetName.data());
	}
	static void Release()
	{
		AssetMap.clear();
	}

	template<typename AssetType>
	ENGINE_API static void AddAsset(std::shared_ptr<UAssetManager> Asset, std::string_view AssetName, std::string_view AssetPath)
	{
		const type_info& Info = typeid(AssetType);
		AddAsset(Asset, Info.name(), AssetName, AssetPath);
	}
	ENGINE_API static void AddAsset(std::shared_ptr<UAssetManager> Asset, const std::string_view InfoName, std::string_view AssetName, std::string_view AssetPath);
	

protected:
	FPaths Path;

private:
	ENGINE_API static inline std::map<std::string, std::map<std::string, std::shared_ptr<UAssetManager>>> AssetMap;
};
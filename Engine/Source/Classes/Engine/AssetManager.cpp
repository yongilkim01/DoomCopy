#include "pch.h"
#include "AssetManager.h"
#include "Classes/Engine/Texture.h"

std::shared_ptr<UAssetManager> UAssetManager::Find(std::string_view AssetName, std::string_view Name)
{
    if (false == AssetMap[AssetName.data()].contains(Name.data()))
    {
        return nullptr;
    }

    return AssetMap[AssetName.data()][Name.data()];
}

ENGINE_API void UAssetManager::AddAsset(std::shared_ptr<UAssetManager> Asset,
    const std::string_view InfoName, std::string_view AssetName, std::string_view AssetPath)
{
    if (true == AssetMap[InfoName.data()].contains(AssetName.data()))
    {
        MSGASSERT("이미 로드한 리소스를 로드하려고 시도 " + std::string(InfoName.data()) + " " + AssetName.data());
        return;
    }

    std::string UpperName = UEngineString::ToUpper(AssetName);

    Asset->SetName(UpperName);
    Asset->Path = AssetPath;
    AssetMap[InfoName.data()].insert({ UpperName, Asset });

    return;
}

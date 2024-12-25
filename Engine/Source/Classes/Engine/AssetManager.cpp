#include "pch.h"
#include "AssetManager.h"
#include "Classes/Engine/Texture.h"

std::shared_ptr<UAssetManager> UAssetManager::Find(std::string_view TypeName, std::string_view AssetName)
{
    std::string UpperAssetName = UEngineString::ToUpper(AssetName);

    if (false == AssetMap[TypeName.data()].contains(UpperAssetName))
    {
        return nullptr;
    }

    return AssetMap[TypeName.data()][UpperAssetName];
}


ENGINE_API void UAssetManager::AddAsset(std::shared_ptr<UAssetManager> Asset,
    const std::string_view TypeName, std::string_view AssetName, std::string_view AssetPath)
{
    if (true == AssetMap[TypeName.data()].contains(AssetName.data()))
    {
        MSGASSERT("�̹� �ε��� ���ҽ��� �ε��Ϸ��� �õ� " + std::string(TypeName.data()) + " " + AssetName.data());
        return;
    }

    std::string UpperName = UEngineString::ToUpper(AssetName);

    Asset->SetName(UpperName);
    Asset->Path = AssetPath;
    AssetMap[TypeName.data()].insert({ UpperName, Asset });

    return;
}

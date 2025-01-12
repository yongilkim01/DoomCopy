#include "pch.h"
#include "Engine/Classes/Engine/RenderAsset.h"

#include "Engine/Classes/Engine/Texture.h"

std::shared_ptr<URenderAsset> URenderAsset::Find(std::string_view TypeName, std::string_view AssetName)
{
    std::string UpperAssetName = UEngineString::ToUpper(AssetName);

    if (false == AssetMap[TypeName.data()].contains(UpperAssetName))
    {
        return nullptr;
    }

    return AssetMap[TypeName.data()][UpperAssetName];
}


ENGINE_API void URenderAsset::AddAsset(std::shared_ptr<URenderAsset> Asset,
    const std::string_view TypeName, std::string_view AssetName, std::string_view AssetPath)
{
    std::string UpperAssetName = UEngineString::ToUpper(AssetName);

    if (true == AssetMap[TypeName.data()].contains(UpperAssetName))
    {
        MSGASSERT("�̹� �ε��� ���ҽ��� �ε��Ϸ��� �õ� " + std::string(TypeName.data()) + " " + AssetName.data());
        return;
    }

    Asset->SetName(UpperAssetName);
    Asset->Path = AssetPath;
    AssetMap[TypeName.data()].insert({ UpperAssetName, Asset });

    return;
}

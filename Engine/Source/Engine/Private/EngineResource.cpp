#include "Engine.h"
#include "Engine/Classes/Engine/EngineResource.h"

#include "Engine/Classes/Engine/Texture.h"

std::shared_ptr<UResource> UResource::Find(std::string_view TypeName, std::string_view AssetName)
{
    std::string UpperAssetName = UEngineString::ToUpper(AssetName);

    if (false == ResourceMap[TypeName.data()].contains(UpperAssetName))
    {
        return nullptr;
    }

    return ResourceMap[TypeName.data()][UpperAssetName];
}


ENGINE_API void UResource::AddAsset(std::shared_ptr<UResource> Asset,
    const std::string_view TypeName, std::string_view AssetName, std::string_view AssetPath)
{
    std::string UpperAssetName = UEngineString::ToUpper(AssetName);

    if (true == ResourceMap[TypeName.data()].contains(UpperAssetName))
    {
        MSGASSERT("�̹� �ε��� ���ҽ��� �ε��Ϸ��� �õ� " + std::string(TypeName.data()) + " " + AssetName.data());
        return;
    }

    Asset->SetName(UpperAssetName);
    Asset->Path = AssetPath;
    ResourceMap[TypeName.data()].insert({ UpperAssetName, Asset });

    return;
}

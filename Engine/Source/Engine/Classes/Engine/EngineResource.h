#pragma once
#include "Core/Object/Object.h"
#include "Core/Misc/Paths.h"
#include "Core/Containers/EngineString.h"

enum class ERenderAssetType
{
    None,
    Texture,
    StaticMesh,
    SkeletalMesh,
};


/**
 *  ����
 */
class UResource : public UObject
{
public:
    /** ������, �Ҹ��� */
    UResource() {}
    ~UResource() {}

    /** ��ü �� ���� ���� */
    UResource(const UResource& Other) = delete;
    UResource(UResource&& Other) noexcept = delete;
    UResource& operator=(const UResource& Other) = delete;
    UResource& operator=(UResource&& Other) noexcept = delete;

    /**
     * Ư�� Ÿ���� Asset�� ã�� �޼ҵ�
     *
     * @param AssetName - ã�� Asset�� �̸�
     * @return ã�� Asset�� ���� ������
     */
    template<typename AssetType>
    ENGINE_API static std::shared_ptr<AssetType> Find(std::string_view AssetName)
    {
        const type_info& Info = typeid(AssetType);
        return std::dynamic_pointer_cast<AssetType>(Find(Info.name(), AssetName.data()));
    }
    /**
     * Ư�� Ÿ�԰� �̸��� Asset�� ã�� �޼ҵ�
     *
     * @param TypeName - Asset�� Ÿ�� �̸�
     * @param AssetName - Asset�� �̸�
     * @return ã�� Asset�� ���� ������
     */
    ENGINE_API static std::shared_ptr<UResource> Find(std::string_view TypeName, std::string_view AssetName);
    /**
     * Asset�� �߰��ϴ� �޼ҵ�
     *
     * @param Asset - �߰��� Asset�� ���� ������
     * @param AssetName - Asset�� �̸�
     * @param AssetPath - Asset�� ���
     */
    template<typename AssetType>
    ENGINE_API static void AddAsset(std::shared_ptr<UResource> Asset, std::string_view AssetName,
        std::string_view AssetPath)
    {
        const type_info& Info = typeid(AssetType);
        AddAsset(Asset, Info.name(), AssetName, AssetPath);
    }
    /**
     * Asset�� �߰��ϴ� �޼ҵ�
     *
     * @param Asset - �߰��� Asset�� ���� ������
     * @param TypeName - Asset�� Ÿ�� �̸�
     * @param AssetName - Asset�� �̸�
     * @param AssetPath - Asset�� ���
     */
    ENGINE_API static void AddAsset(std::shared_ptr<UResource> Asset, const std::string_view TypeName,
        std::string_view AssetName, std::string_view AssetPath);

    /**
     * ���ڿ��� �빮�ڷ� ��ȯ�ϴ� �޼ҵ�
     *
     * @param Name - ��ȯ�� ���ڿ�
     * @return ��ȯ�� �빮�� ���ڿ�
     */
    static std::string ToUpperName(std::string_view Name)
    {
        return UEngineString::ToUpper(Name);
    }
    /**
     * Asset�� �����ϴ��� Ȯ���ϴ� �޼ҵ�
     *
     * @param AssetName - Ȯ���� Asset�� �̸�
     * @return Asset ���� ����
     */
    static bool Contains(std::string_view AssetName)
    {
        return ResourceMap.contains(AssetName.data());
    }
    /**
     * ��� Asset�� �����ϴ� �޼ҵ�
     */
    static void Release()
    {
        ResourceMap.clear();
    }

    /** ��, �� �޼ҵ� */
    ENGINE_API FPaths GetPath()
    {
        return Path;
    }

protected:
    /** Asset�� ��� */
    FPaths Path;

private:
    /** Asset�� �����ϴ� �� */
    ENGINE_API static inline std::map<std::string, std::map<std::string, std::shared_ptr<UResource>>> ResourceMap;
};

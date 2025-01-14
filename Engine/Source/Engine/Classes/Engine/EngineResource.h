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
 *  설명
 */
class UResource : public UObject
{
public:
    /** 생성자, 소멸자 */
    UResource() {}
    ~UResource() {}

    /** 객체 값 복사 방지 */
    UResource(const UResource& Other) = delete;
    UResource(UResource&& Other) noexcept = delete;
    UResource& operator=(const UResource& Other) = delete;
    UResource& operator=(UResource&& Other) noexcept = delete;

    /**
     * 특정 타입의 Asset을 찾는 메소드
     *
     * @param AssetName - 찾을 Asset의 이름
     * @return 찾은 Asset의 공유 포인터
     */
    template<typename AssetType>
    ENGINE_API static std::shared_ptr<AssetType> Find(std::string_view AssetName)
    {
        const type_info& Info = typeid(AssetType);
        return std::dynamic_pointer_cast<AssetType>(Find(Info.name(), AssetName.data()));
    }
    /**
     * 특정 타입과 이름의 Asset을 찾는 메소드
     *
     * @param TypeName - Asset의 타입 이름
     * @param AssetName - Asset의 이름
     * @return 찾은 Asset의 공유 포인터
     */
    ENGINE_API static std::shared_ptr<UResource> Find(std::string_view TypeName, std::string_view AssetName);
    /**
     * Asset을 추가하는 메소드
     *
     * @param Asset - 추가할 Asset의 공유 포인터
     * @param AssetName - Asset의 이름
     * @param AssetPath - Asset의 경로
     */
    template<typename AssetType>
    ENGINE_API static void AddAsset(std::shared_ptr<UResource> Asset, std::string_view AssetName,
        std::string_view AssetPath)
    {
        const type_info& Info = typeid(AssetType);
        AddAsset(Asset, Info.name(), AssetName, AssetPath);
    }
    /**
     * Asset을 추가하는 메소드
     *
     * @param Asset - 추가할 Asset의 공유 포인터
     * @param TypeName - Asset의 타입 이름
     * @param AssetName - Asset의 이름
     * @param AssetPath - Asset의 경로
     */
    ENGINE_API static void AddAsset(std::shared_ptr<UResource> Asset, const std::string_view TypeName,
        std::string_view AssetName, std::string_view AssetPath);

    /**
     * 문자열을 대문자로 변환하는 메소드
     *
     * @param Name - 변환할 문자열
     * @return 변환된 대문자 문자열
     */
    static std::string ToUpperName(std::string_view Name)
    {
        return UEngineString::ToUpper(Name);
    }
    /**
     * Asset이 존재하는지 확인하는 메소드
     *
     * @param AssetName - 확인할 Asset의 이름
     * @return Asset 존재 여부
     */
    static bool Contains(std::string_view AssetName)
    {
        return ResourceMap.contains(AssetName.data());
    }
    /**
     * 모든 Asset을 해제하는 메소드
     */
    static void Release()
    {
        ResourceMap.clear();
    }

    /** 겟, 셋 메소드 */
    ENGINE_API FPaths GetPath()
    {
        return Path;
    }

protected:
    /** Asset의 경로 */
    FPaths Path;

private:
    /** Asset을 저장하는 맵 */
    ENGINE_API static inline std::map<std::string, std::map<std::string, std::shared_ptr<UResource>>> ResourceMap;
};

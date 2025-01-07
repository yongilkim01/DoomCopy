#pragma once
#include "SceneComponent.h"

/**
 *	설명
 */
class UShapeComponent : public USceneComponent
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UShapeComponent();
	ENGINE_API ~UShapeComponent();

	/** 객체 값 복사 방지 */
	UShapeComponent(const UShapeComponent& Other) = delete;
	UShapeComponent(UShapeComponent&& Other) noexcept = delete;
	UShapeComponent& operator=(const UShapeComponent& Other) = delete;
	UShapeComponent& operator=(UShapeComponent&& Other) noexcept = delete;

	ENGINE_API virtual void BeginPlay() override;

	ENGINE_API bool CollisionCheck(std::string_view _OtherName, std::vector<UShapeComponent*>& ShapeVector);

	/** 겟, 셋 메소드 */
	ENGINE_API std::string GetCollisionProfileName() { return ProfileName; }
	ENGINE_API void SetCollisionProfileName(std::string_view NewProfileName);
	void SetCollisionType(ECollisionType NewCollisionType)
	{
		CollisionType = NewCollisionType;
	}
protected:

private:
	std::string ProfileName = "";

	ECollisionType CollisionType = ECollisionType::AABB;

};


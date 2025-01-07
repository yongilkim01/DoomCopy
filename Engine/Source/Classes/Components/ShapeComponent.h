#pragma once
#include "SceneComponent.h"

/**
 *	����
 */
class UShapeComponent : public USceneComponent
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UShapeComponent();
	ENGINE_API ~UShapeComponent();

	/** ��ü �� ���� ���� */
	UShapeComponent(const UShapeComponent& Other) = delete;
	UShapeComponent(UShapeComponent&& Other) noexcept = delete;
	UShapeComponent& operator=(const UShapeComponent& Other) = delete;
	UShapeComponent& operator=(UShapeComponent&& Other) noexcept = delete;

	ENGINE_API virtual void BeginPlay() override;

	/** ��, �� �޼ҵ� */
	ENGINE_API std::string GetCollisionProfileName() { return ProfileName; }
	ENGINE_API void SetCollisionProfileName(std::string_view NewProfileName);

protected:

private:
	std::string ProfileName = "";

	ECollisionType CollisionType = ECollisionType::AABB;

};


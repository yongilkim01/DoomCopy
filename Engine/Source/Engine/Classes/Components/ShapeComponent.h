#pragma once
#include "SceneComponent.h"
#include "Engine/Classes/Engine/PaperSprite.h"
#include "Engine/Classes/Engine/RenderAsset.h"
#include <set>

class UCameraComponent;

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
	ENGINE_API void DebugRender(UCameraComponent* CameraComponent, float DeltaTime);
	ENGINE_API bool CollisionCheck(std::string_view OtherName, FVector NextLocation, std::vector<UShapeComponent*>& _Vector);
	ENGINE_API bool CollisionCheck(std::string_view OtherName, std::vector<UShapeComponent*>& ShapeVector);
	ENGINE_API void CollisionEventCheck(std::shared_ptr<UShapeComponent> OtherComp);

	/** ��, �� �޼ҵ� */
	ENGINE_API void SetRadius(float Radius);
	ENGINE_API std::string GetCollisionProfileName() { return ProfileName; }
	ENGINE_API void SetCollisionProfileName(std::string_view NewProfileName);
	void SetCollisionType(ECollisionType NewCollisionType)
	{
		CollisionType = NewCollisionType;
	}
	bool IsEvent()
	{
		return Enter != nullptr || Stay != nullptr || End != nullptr;
	}
protected:

private:
	ENGINE_API void SetCollisionEnter(std::function<void(UShapeComponent*, UShapeComponent*)> EventFunc);
	ENGINE_API void SetCollisionStay(std::function<void(UShapeComponent*, UShapeComponent*)> EventFunc);
	ENGINE_API void SetCollisionEnd(std::function<void(UShapeComponent*, UShapeComponent*)> EventFunc);

	std::string ProfileName = "NONE";
	ECollisionType CollisionType = ECollisionType::OBB2D;
	std::set<UShapeComponent*> CollisionCheckSet;

	// ���� �浹
	std::function<void(UShapeComponent*, UShapeComponent*)> Enter;
	// ���� �浹 <= �𸮾��� �̰� �����ϴ�.
	std::function<void(UShapeComponent*, UShapeComponent*)> Stay;
	// �浹 ��.
	std::function<void(UShapeComponent*, UShapeComponent*)> End;
};


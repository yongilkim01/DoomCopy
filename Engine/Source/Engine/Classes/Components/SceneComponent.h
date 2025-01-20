#pragma once
#include "Engine/Classes/Components/ActorComponent.h"
#include "Engine/Classes/Engine/TransformObject.h"
#include "Core/Public/Math/EngineMath.h"

class AActor;

/**
 *	����
 */
class USceneComponent : public UActorComponent, public UTransformObject
{
	friend AActor;
public:
	/** ������, �Ҹ��� */
	ENGINE_API USceneComponent();
	ENGINE_API ~USceneComponent();

	/** ��ü �� ���� ���� */
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	ENGINE_API virtual void TickComponent(float DeltaTime) override;

protected:
	ENGINE_API virtual void BeginPlay() override;

private:

};


#pragma once
#include "Engine/Classes/Components/ActorComponent.h"

class AActor;

/**
 *	����
 */
class UMovementComponent : public UActorComponent
{
	friend AActor;
public:
	/** ������, �Ҹ��� */
	ENGINE_API UMovementComponent();
	ENGINE_API ~UMovementComponent();

	/** ��ü �� ���� ���� */
	UMovementComponent(const UMovementComponent& Other) = delete;
	UMovementComponent(UMovementComponent&& Other) noexcept = delete;
	UMovementComponent& operator=(const UMovementComponent& Other) = delete;
	UMovementComponent& operator=(UMovementComponent&& Other) noexcept = delete;

	ENGINE_API virtual void ComponentTick(float DeltaTime) override;

protected:
	ENGINE_API virtual void BeginPlay() override;

private:

};


#pragma once
#include "Engine/Classes/Components/ActorComponent.h"

class AActor;

/**
 *	설명
 */
class UMovementComponent : public UActorComponent
{
	friend AActor;
public:
	/** 생성자, 소멸자 */
	ENGINE_API UMovementComponent();
	ENGINE_API ~UMovementComponent();

	/** 객체 값 복사 방지 */
	UMovementComponent(const UMovementComponent& Other) = delete;
	UMovementComponent(UMovementComponent&& Other) noexcept = delete;
	UMovementComponent& operator=(const UMovementComponent& Other) = delete;
	UMovementComponent& operator=(UMovementComponent&& Other) noexcept = delete;

	ENGINE_API virtual void ComponentTick(float DeltaTime) override;

protected:
	ENGINE_API virtual void BeginPlay() override;

private:

};


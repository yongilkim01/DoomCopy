#pragma once
#include "Engine/Classes/Components/ShapeComponent.h"

class UCameraComponent;

/**
 *	설명
 */
class UCapsuleComponent : public UShapeComponent
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UCapsuleComponent();
	ENGINE_API ~UCapsuleComponent();

	/** 객체 값 복사 방지 */
	UCapsuleComponent(const UCapsuleComponent& Other) = delete;
	UCapsuleComponent(UCapsuleComponent&& Other) noexcept = delete;
	UCapsuleComponent& operator=(const UCapsuleComponent& Other) = delete;
	UCapsuleComponent& operator=(UCapsuleComponent&& Other) noexcept = delete;

	ENGINE_API virtual void BeginPlay() override;
	ENGINE_API virtual bool MoveComponent(const FVector& Delta, const FVector& NewRotation = FVector::ZERO, bool bSweep = true) override;
	ENGINE_API virtual FVector SweepCollision(const FVector& NewLocation, const FVector& Delta) override;


	ENGINE_API void SetCapsuleSize(float InRadius, float InHalfHeight)
	{
		CapsuleRadius = InRadius;
		CapsuleHalfHeight = InHalfHeight;
	}

protected:

private:
	float CapsuleHalfHeight;
	float CapsuleRadius;
};


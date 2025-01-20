#pragma once
#include "Engine/Classes/Components/ActorComponent.h"

class UPrimitiveComponent;
class ACharacter;

/**
 *	설명
 */
class UCharacterMovementComponent : public UActorComponent
{
	friend ACharacter;
public:
	/** 생성자, 소멸자 */
	ENGINE_API UCharacterMovementComponent();
	ENGINE_API ~UCharacterMovementComponent();

	/** 객체 값 복사 방지 */
	UCharacterMovementComponent(const UCharacterMovementComponent& Other) = delete;
	UCharacterMovementComponent(UCharacterMovementComponent&& Other) noexcept = delete;
	UCharacterMovementComponent& operator=(const UCharacterMovementComponent& Other) = delete;
	UCharacterMovementComponent& operator=(UCharacterMovementComponent&& Other) noexcept = delete;

	ENGINE_API virtual void BeginPlay() override;
	ENGINE_API virtual void TickComponent(float DeltaTime) override;

	ENGINE_API bool SafeMoveUpdatedComponent(const FVector& Delta, const FVector& NewRotation, bool bSweep);
	ENGINE_API void AddInputVector(FVector WorldVector, bool bForce = false);

protected:
	///** Perform movement on an autonomous client */
	//ENGINE_API virtual void PerformMovement(float DeltaTime);

	///** Special Tick for Simulated Proxies */
	//ENGINE_API virtual void SimulatedTick(float DeltaSeconds);

	///** Simulate movement on a non-owning client. Called by SimulatedTick(). */
	//ENGINE_API virtual void SimulateMovement(float DeltaTime);

private:
	FVector PendingInputVector = FVector::ZERO;
	FVector Velocity = FVector::ZERO;
	UPrimitiveComponent* UpdatedComponent = nullptr;
	float JumpZVelocity = 600.0f;
	float Gravity = 0.0f;
	float GravityScale = 0.0f;
	float MaxAcceleration = 1.0f;
	float MaxWalkSpeed = 200.0f;
};


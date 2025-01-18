#pragma once
#include "Engine/Classes/Components/ActorComponent.h"

class UPrimitiveComponent;
class ACharacter;

/**
 *	����
 */
class UCharacterMovementComponent : public UActorComponent
{
	friend ACharacter;
public:
	/** ������, �Ҹ��� */
	ENGINE_API UCharacterMovementComponent();
	ENGINE_API ~UCharacterMovementComponent();

	/** ��ü �� ���� ���� */
	UCharacterMovementComponent(const UCharacterMovementComponent& Other) = delete;
	UCharacterMovementComponent(UCharacterMovementComponent&& Other) noexcept = delete;
	UCharacterMovementComponent& operator=(const UCharacterMovementComponent& Other) = delete;
	UCharacterMovementComponent& operator=(UCharacterMovementComponent&& Other) noexcept = delete;

	ENGINE_API virtual void BeginPlay() override;
	ENGINE_API virtual void ComponentTick(float DeltaTime) override;

	ENGINE_API bool SafeMoveUpdatedComponent(const FVector& Delta, const FVector& NewRotation, bool bSweep);
	ENGINE_API bool MoveUpdatedComponent(const FVector& Delta, const FVector& NewRotation, bool bSweep/*,FHitResult* Hit*/);
	ENGINE_API void AddInputVector(FVector WorldVector, bool bForce = false);
	ENGINE_API void ApplyGravity();
	ENGINE_API void PeformJump();
	ENGINE_API void CalculateMoveDirection();
	ENGINE_API void UpdateCharacterPosition(FVector MoveDirection);
	ENGINE_API void HandleCollisions();
	ENGINE_API void FinalizeMovement();
	ENGINE_API bool IsMovingOnGround();
	ENGINE_API bool IsFalling();

protected:
	/** Perform movement on an autonomous client */
	ENGINE_API virtual void PerformMovement(float DeltaTime);

	/** Special Tick for Simulated Proxies */
	ENGINE_API virtual void SimulatedTick(float DeltaSeconds);

	/** Simulate movement on a non-owning client. Called by SimulatedTick(). */
	ENGINE_API virtual void SimulateMovement(float DeltaTime);

private:
	FVector PendingInputVector = FVector::ZERO;
	FVector Velocity = FVector::ZERO;
	UPrimitiveComponent* UpdatedComponent = nullptr;
	float JumpZVelocity = 600.0f;
	float Gravity = 0.0f;
	float GravityScale = 0.0f;
	float MaxAcceleration = 1.0f;
	float MaxWalkSpeed = 100.0f;
};


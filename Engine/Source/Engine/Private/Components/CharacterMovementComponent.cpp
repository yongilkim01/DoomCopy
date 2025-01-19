#include "pch.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"

UCharacterMovementComponent::UCharacterMovementComponent()
{
}

UCharacterMovementComponent::~UCharacterMovementComponent()
{
}

void UCharacterMovementComponent::BeginPlay()
{
}

void UCharacterMovementComponent::ComponentTick(float DeltaTime)
{
    // 입력 벡터를 기반으로 가속도 계산
    FVector Acceleration = PendingInputVector.GetClampedToMaxSize(100.0f) * MaxAcceleration;

    // 속도 계산 (가속도 적용)
    //Velocity += Acceleration * DeltaTime;
    Velocity = Acceleration;

    // 속도 제한
    Velocity = Velocity.GetClampedToMaxSize(MaxWalkSpeed);

    // 이동 벡터 계산
    FVector Delta = Velocity * DeltaTime;

    // 캡슐 컴포넌트 이동 및 충돌 처리
    SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentRotation(), true); 
}

void UCharacterMovementComponent::PerformMovement(float DeltaTime)
{
	FVector DesiredMovement = Velocity * DeltaTime;

	Velocity.Z -= Gravity * GravityScale * DeltaTime;
}

void UCharacterMovementComponent::SimulatedTick(float DeltaSeconds)
{

}

void UCharacterMovementComponent::SimulateMovement(float DeltaTime)
{

}

bool UCharacterMovementComponent::SafeMoveUpdatedComponent(const FVector& Delta, const FVector& NewRotation, bool bSweep)
{
    //FHitResult Hit;

    // 이동 시도
    bool bMoved = MoveUpdatedComponent(Delta, NewRotation, bSweep/*, &Hit*/);

    // TODO: 충돌 처리
    //if (Hit.IsValidBlockingHit())
    //{
    //    SlideAlongSurface(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);
    //}

    return bMoved;
}

bool UCharacterMovementComponent::MoveUpdatedComponent(const FVector& Delta, const FVector& NewRotation, bool bSweep)
{
    PendingInputVector = FVector::ZERO;

    return UpdatedComponent->MoveComponent(Delta, NewRotation, bSweep);
}

void UCharacterMovementComponent::AddInputVector(FVector WorldVector, bool bForce)
{
	PendingInputVector += WorldVector;

	//TODO: 강제로 갱신할 경우 마지막 입력도 저장
}

void UCharacterMovementComponent::ApplyGravity()
{
}

void UCharacterMovementComponent::PeformJump()
{
}

void UCharacterMovementComponent::CalculateMoveDirection()
{
}

void UCharacterMovementComponent::UpdateCharacterPosition(FVector MoveDirection)
{
}

void UCharacterMovementComponent::HandleCollisions()
{
}

void UCharacterMovementComponent::FinalizeMovement()
{
}

bool UCharacterMovementComponent::IsMovingOnGround()
{
	return false;
}

bool UCharacterMovementComponent::IsFalling()
{
	return false;
}

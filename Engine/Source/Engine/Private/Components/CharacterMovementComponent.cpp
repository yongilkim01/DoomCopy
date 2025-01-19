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
    // �Է� ���͸� ������� ���ӵ� ���
    FVector Acceleration = PendingInputVector.GetClampedToMaxSize(100.0f) * MaxAcceleration;

    // �ӵ� ��� (���ӵ� ����)
    //Velocity += Acceleration * DeltaTime;
    Velocity = Acceleration;

    // �ӵ� ����
    Velocity = Velocity.GetClampedToMaxSize(MaxWalkSpeed);

    // �̵� ���� ���
    FVector Delta = Velocity * DeltaTime;

    // ĸ�� ������Ʈ �̵� �� �浹 ó��
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

    // �̵� �õ�
    bool bMoved = MoveUpdatedComponent(Delta, NewRotation, bSweep/*, &Hit*/);

    // TODO: �浹 ó��
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

	//TODO: ������ ������ ��� ������ �Էµ� ����
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

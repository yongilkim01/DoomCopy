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
    Velocity = PendingInputVector;

    // �ӵ� ����
    Velocity = Velocity.GetClampedToMaxSize(MaxWalkSpeed);

    // �߷� �߰�
    Velocity.Y -= 500.0f;

    // �̵� ���� ���
    FVector Delta = Velocity * DeltaTime;

    // ĸ�� ������Ʈ �̵� �� �浹 ó��
    SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentRotation(), true); 
}

bool UCharacterMovementComponent::SafeMoveUpdatedComponent(const FVector& Delta, const FVector& NewRotation, bool bSweep)
{
    PendingInputVector = FVector::ZERO;

    return UpdatedComponent->MoveComponent(Delta, NewRotation, bSweep);
}

void UCharacterMovementComponent::AddInputVector(FVector WorldVector, bool bForce)
{
	PendingInputVector += WorldVector;

	//TODO: ������ ������ ��� ������ �Էµ� ����
}
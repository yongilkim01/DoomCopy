#include "Engine.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"

#include "Core/Public/Debugging/DebugMacros.h"

UCharacterMovementComponent::UCharacterMovementComponent()
{
}

UCharacterMovementComponent::~UCharacterMovementComponent()
{
}

void UCharacterMovementComponent::BeginPlay()
{
    PendingInputVector = FVector::ZERO;
}

void UCharacterMovementComponent::TickComponent(float DeltaTime)    
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

    Velocity.Y = 0.0f;

    //UEngineDebug::OutPutString("Player Velocity : " + Velocity.ToString());
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
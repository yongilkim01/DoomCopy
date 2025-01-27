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
    // 입력 벡터를 기반으로 가속도 계산
    Velocity = PendingInputVector;

    // 속도 제한
    Velocity = Velocity.GetClampedToMaxSize(MaxWalkSpeed);

    // 중력 추가
    Velocity.Y -= 500.0f;

    // 이동 벡터 계산
    FVector Delta = Velocity * DeltaTime;

    // 캡슐 컴포넌트 이동 및 충돌 처리
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

	//TODO: 강제로 갱신할 경우 마지막 입력도 저장
}
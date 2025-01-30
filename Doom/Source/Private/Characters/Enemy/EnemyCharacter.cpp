#include "Doom.h"
#include "Public/Characters/Enemy/EnemyCharacter.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

AEnemyCharacter::AEnemyCharacter()
{
}

AEnemyCharacter::~AEnemyCharacter()
{
}

FVector AEnemyCharacter::GetDirectionToTargetLocation(FVector TargetLocation)
{
	FVector ToTarget = TargetLocation - GetActorLocation();
	ToTarget.Normalize(); // ���� ���� ����ȭ

	// ĳ������ ���� ��ǥ ���� ���� ���� ���
	FVector ForwardDir = FVector(0.0f, 0.0f, 1.0f);  // ���� (X)
	FVector RightDir = FVector(1.0f, 0.0f, 0.0f);  // ���� (X)

	// �� �ະ�� ���� ������� ū ���� ����
	float ForwardDot = FVector::Dot(ToTarget, ForwardDir);
	float RightDot = FVector::Dot(ToTarget, RightDir);

	// ���� ū ���� ��� ������ �Ǵ��Ͽ� ���� ����
	if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
	{
		return (ForwardDot > 0.0f) ? ForwardDir : -ForwardDir;  // Forward / Backward
	}
	else
	{
		return (RightDot > 0.0f) ? RightDir : -RightDir;        // Right / Left
	}
}


void AEnemyCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);

	switch (CurEnemyState)
	{
	case EEnemyState::PATROL:
		Patrol(DeltaTime);
		break;
	case EEnemyState::ATTACK:
		Attack(DeltaTime);
		break;
	case EEnemyState::TRACE:
		Trace(DeltaTime);
		break;
	case EEnemyState::DEATH:
		Death(DeltaTime);
		break;
	default:
		break;
	}
}

void AEnemyCharacter::ChangeState(EEnemyState State)
{
	switch (State)
	{
	case EEnemyState::PATROL:
		EntryPatrol();
		break;
	case EEnemyState::ATTACK:
		EntryPatrol();
		break;
	case EEnemyState::TRACE:
		EntryTrace();
		break;
	case EEnemyState::DEATH:
		EntryDeath();
		break;
	default:
		break;
	}

	PrevEnemyState = CurEnemyState;
	CurEnemyState = State;
}

void AEnemyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = GetActorForwardVector();

		AddMovementInput(Direction, Value);
	}
}

void AEnemyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = GetActorRightVector();

		AddMovementInput(Direction, Value);
	}
}
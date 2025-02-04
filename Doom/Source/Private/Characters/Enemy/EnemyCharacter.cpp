#include "Doom.h"
#include "Public/Characters/Enemy/EnemyCharacter.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>\

#include "Public/Items/DropItems/DropShotgun.h"

AEnemyCharacter::AEnemyCharacter()
{
}

AEnemyCharacter::~AEnemyCharacter()
{
}

FVector AEnemyCharacter::GetDirectionToTargetLocation(FVector TargetLocation)
{
	FVector ToTarget = TargetLocation - GetActorLocation();
	ToTarget.Normalize(); // 방향 벡터 정규화

	// 캐릭터의 로컬 좌표 기준 방향 벡터 얻기
	FVector ForwardDir = FVector(0.0f, 0.0f, 1.0f);  // 전방 (X)
	FVector RightDir = FVector(1.0f, 0.0f, 0.0f);  // 전방 (X)

	// 각 축별로 가장 영향력이 큰 방향 결정
	float ForwardDot = FVector::Dot(ToTarget, ForwardDir);
	float RightDot = FVector::Dot(ToTarget, RightDir);

	// 가장 큰 값이 어느 축인지 판단하여 방향 결정
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
	case EEnemyState::EXP_DEATH:
		ExpDeath(DeltaTime);
		break;
	default:
		break;
	}
}

void AEnemyCharacter::ChangeState(EEnemyState State)
{
	PrevEnemyState = CurEnemyState;
	CurEnemyState = State;

	switch (State)
	{
	case EEnemyState::PATROL:
		EntryPatrol();
		break;
	case EEnemyState::ATTACK:
		EntryAttack();
		break;
	case EEnemyState::TRACE:
		EntryTrace();
		break;
	case EEnemyState::DEATH:
		EntryDeath();
		break;
	case EEnemyState::EXP_DEATH:
		EntryExpDeath();
		break;
	default:
		break;
	}

}

bool AEnemyCharacter::CheckActorInRange(AActor* TargetActor)
{
	FVector ToTargetVector = TargetActor->GetActorLocation() - GetActorLocation();
	float DotDegree = FVector::GetVectorAngleDeg(GetCurDirection(), ToTargetVector);
	float Distance = FVector::DistXZ(GetActorLocation(), TargetActor->GetActorLocation());

	//UEngineDebug::OutPutString("DotDegree : " + std::to_string(DotDegree));
	UEngineDebug::OutPutString("Distance : " + std::to_string(Distance));

	if (0 < DotDegree && DotDegree < 50.0f)
	{
		if (Distance < DetectRange)
		{
			//UEngineDebug::OutPutString("Player Detected!!");
			//this;
			//TargetActor;
			return true;
		}
	}

	return false;
}

void AEnemyCharacter::DropItem()
{
	if (EDropItemType::NONE == DropItemType)
	{
		return;
	}

	switch (DropItemType)
	{
	case EDropItemType::SHOTGUN:
	{
		std::shared_ptr<ADropShotgun> Armor = GetWorld()->SpawnActor<ADropShotgun>();
		Armor->SetActorLocation(GetActorLocation() + (GetCurDirection() * 15.0f));
		Armor->AddActorLocation(FVector{ 0.0f, -20.0f, 0.0f });
		break;
	}
	default:
		break;
	}
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
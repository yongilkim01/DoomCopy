#include "Doom.h"
#include "Public/Characters/Enemy/ZombieCharacter.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>

AZombieCharacter::AZombieCharacter()
{
	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetRelativeScale3D({ 120.0f, 120.0f });
	SpriteComponent->SetAutoScale(false);
	SpriteComponent->OnBillboard();
	SpriteComponent->CreateAnimation("Move_Forward", "DoomZombie.png", 0, 3, 0.5f, true);
	SpriteComponent->CreateAnimation("Move_Left", "DoomZombie.png", 12, 15, 0.5f, true);
	SpriteComponent->CreateAnimation("Move_Right", "DoomZombie.png", 30, 33, 0.5f, true);
	SpriteComponent->CreateAnimation("Move_Back", "DoomZombie.png", 42, 45, 0.5f, true);

	SpriteComponent->ChangeAnimation("Move_Forward");

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetRelativeScale3D({ 30.0f, 30.0f, 30.0f });
	ShapeComponent->SetCollisionProfileName("EnemyBody");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);

	CapsuleComponent->SetCapsuleSize(5.0f, 41.0f);
}

AZombieCharacter::~AZombieCharacter()
{
}

void AZombieCharacter::BeginPlay()
{
    AEnemyCharacter::BeginPlay();

    TurningLocations.push_back(FVector{ 30.0f, 130.0f, 3179.0f });
    TurningLocations.push_back(FVector{ 30.0f, 130.0f, 3300.0f });
    TurningLocations.push_back(FVector{ 130.0f, 130.0f, 3300.0f });
    TurningLocations.push_back(FVector{ 130.0f, 130.0f, 3179.0f });

    ChangeState(EEnemyState::PATROL);
}

void AZombieCharacter::Tick(float DeltaTime)
{
	AEnemyCharacter::Tick(DeltaTime);


}

void AZombieCharacter::EntryPatrol()
{
	SetSpeed(100.0f);
}

void AZombieCharacter::EntryAttack()
{
}

void AZombieCharacter::EntryTrace()
{
}

void AZombieCharacter::EntryDeath()
{
}

void AZombieCharacter::Patrol(float DeltaTime)
{
    if (FVector::Dist(GetActorLocation(), TurningLocations[CurTurningIndex]) < 5.0f)
    {
        // 다음 타겟 위치 설정
        if (CurTurningIndex == TurningLocations.size() - 1)
        {
            CurTurningIndex = 0;
        }
        else
        {
            CurTurningIndex++;
        }

        // 현재 이동 방향 업데이트
        SetCurDirection(GetDirectionToTargetLocation(TurningLocations[CurTurningIndex]));
    }
    else
    {
        float DotDegree = FVector::GetVectorAngleDeg(GetCurDirection(), (GetWorld()->GetMainPawn()->GetActorLocation() - GetActorLocation()));
        ChangeAnimation(DotDegree);

        FVector TurningLocation = this->TurningLocations[CurTurningIndex];

        FVector MoveDir = TurningLocation - GetActorLocation();
        MoveDir.Normalize();

        // 전진 또는 후진
        if (MoveDir.Z > 0)
        {
            MoveForward(Speed);  // 전진
        }
        else
        {
            MoveForward(-Speed); // 후진
        }

        // 좌측 또는 우측 이동
        if (MoveDir.X > 0)
        {
            MoveRight(Speed);  // 우측 이동
        }
        else
        {
            MoveRight(-Speed); // 좌측 이동
        }
        
    }

    // 상태 머신 변경 메소드
    if (true == CheckActorInRange(GetWorld()->GetMainPawn()))
    {
        ChangeState(EEnemyState::ATTACK);
    }
}

void AZombieCharacter::Attack(float DeltaTime)
{
}

void AZombieCharacter::Trace(float DeltaTime)
{
}

void AZombieCharacter::Death(float DeltaTime)
{
}

void AZombieCharacter::ChangeAnimation(FVector Direction)
{
    switch (CurEnemyState)
    {
    case EEnemyState::PATROL:
        break;
    case EEnemyState::ATTACK:
        break;
    case EEnemyState::TRACE:
        break;
    case EEnemyState::DEATH:
        break;
    default:
        break;
    }
}

void AZombieCharacter::ChangeAnimation(float Degree)
{
    FVector ToPlayerDirection = GetWorld()->GetMainPawn()->GetActorLocation() - GetActorLocation();
    FVector CrossVector = FVector::Cross(GetCurDirection(), ToPlayerDirection);

    std::string AnimationName;

    switch (CurEnemyState)
    {
    case EEnemyState::PATROL:
    case EEnemyState::TRACE:
        AnimationName = "Move_";
        break;
    case EEnemyState::ATTACK:
        AnimationName = "Attack";
        break;
    case EEnemyState::DEATH:
        AnimationName = "Death_";
        break;
    default:
        break;
    }


    if (45.0f <= Degree && Degree < 135.0f)
    {
        if (CrossVector.Y < 0.0f)
        {
            AnimationName += "Left";
        }
        else
        {
            AnimationName += "Right";
        }

    }
    else if (135.0f <= Degree && Degree < 225.0f)
    {
        AnimationName += "Back";
    }
    else
    {
        AnimationName += "Forward";
    }


    // 최종 애니메이션 적용
    SpriteComponent->ChangeAnimation(AnimationName);
}

#include "Doom.h"
#include "Public/Characters/Enemy/ImpCharacter.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>

#include "Public/Items/Weapons/ZombieBullet.h"

AImpCharacter::AImpCharacter()
{
    SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
    SpriteComponent->SetupAttachment(RootComponent);
    SpriteComponent->SetRelativeScale3D({ 120.0f, 120.0f });
    SpriteComponent->SetAutoScale(false);
    SpriteComponent->OnBillboard();
    SpriteComponent->CreateAnimation("Move_Forward", "DoomImp.png", 0, 3, 0.5f, true);
    SpriteComponent->CreateAnimation("Move_Left", "DoomImp.png", 14, 17, 0.5f, true);
    SpriteComponent->CreateAnimation("Move_Right", "DoomImp.png", 35, 38, 0.5f, true);
    SpriteComponent->CreateAnimation("Move_Back", "DoomImp.png", 49, 52, 0.5f, true);

    SpriteComponent->CreateAnimation("Attack_Forward", "DoomImp.png", 4, 6, 0.5f, false);
    SpriteComponent->CreateAnimation("Attack_Left", "DoomImp.png", 17, 19, 0.5f, false);
    SpriteComponent->CreateAnimation("Attack_Right", "DoomImp.png", 39, 41, 0.5f, false);
    SpriteComponent->CreateAnimation("Attack_Back", "DoomImp.png", 53, 55, 0.5f, false);

    SpriteComponent->CreateAnimation("ExpDeath_Forward", "DoomImp.png", 66, 73, 0.1f, false);
    SpriteComponent->CreateAnimation("ExpDeath_Left", "DoomImp.png", 66, 73, 0.1f, false);
    SpriteComponent->CreateAnimation("ExpDeath_Right", "DoomImp.png", 66, 73, 0.1f, false);
    SpriteComponent->CreateAnimation("ExpDeath_Back", "DoomImp.png", 66, 73, 0.1f, false);

    SpriteComponent->CreateAnimation("Death_Forward", "DoomImp.png", 63, 65, 0.1f, false);
    SpriteComponent->CreateAnimation("Death_Left", "DoomImp.png", 63, 65, 0.1f, false);
    SpriteComponent->CreateAnimation("Death_Right", "DoomImp.png", 63, 65, 0.1f, false);
    SpriteComponent->CreateAnimation("Death_Back", "DoomImp.png", 63, 65, 0.1f, false);


    SpriteComponent->ChangeAnimation("Move_Forward");

    ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
    ShapeComponent->SetupAttachment(RootComponent);
    ShapeComponent->SetRelativeScale3D({ 30.0f, 30.0f, 30.0f });
    ShapeComponent->SetCollisionProfileName("EnemyBody");
    ShapeComponent->SetCollisionType(ECollisionType::Sphere);

    CapsuleComponent->SetCapsuleSize(0.0f, 41.0f);
}

AImpCharacter::~AImpCharacter()
{
}

void AImpCharacter::BeginPlay()
{
    AEnemyCharacter::BeginPlay();

    ChangeState(EEnemyState::PATROL);

    DetectRange = 800.0f;
}

void AImpCharacter::Tick(float DeltaTime)
{
    AEnemyCharacter::Tick(DeltaTime);

    //std::string DebugMsg = "Current Enemy State : ";

    //switch (CurEnemyState)
    //{
    //case EEnemyState::PATROL:
    //    DebugMsg += "PATROL";
    //    break;
    //case EEnemyState::ATTACK:
    //    DebugMsg += "ATTACK";
    //    break;
    //case EEnemyState::TRACE:
    //    DebugMsg += "TRACE";
    //    break;
    //case EEnemyState::DEATH:
    //    DebugMsg += "DEATH";
    //    break;
    //case EEnemyState::EXP_DEATH:
    //    DebugMsg += "EXP_DEATH";
    //    break;
    //default:
    //    break;
    //}

    //UEngineDebug::OutPutString(DebugMsg);
    //UEngineDebug::OutPutString("Cur Index : " + std::to_string(CurTurningIndex));

}

void AImpCharacter::EntryPatrol()
{
    SetSpeed(100.0f);
    CheckTime = 0.0f;
}

void AImpCharacter::EntryAttack()
{
    ChangeAnimation();
    CheckTime = 0.0f;

    std::shared_ptr<AZombieBullet> Bullet = GetWorld()->SpawnActor<AZombieBullet>();
    FVector BulletLocation = GetActorLocation() + GetCurDirection();
    Bullet->SetActorLocation(BulletLocation);
    Bullet->SetEnemyProjectileDirection(GetWorld()->GetMainPawn()->GetActorLocation() - GetActorLocation());
}

void AImpCharacter::EntryTrace()
{
    ChangeAnimation();
    CheckTime = 0.0f;
}

void AImpCharacter::EntryDeath()
{
    ChangeAnimation();
    CheckTime = 0.0f;
    DropItem();
}

void AImpCharacter::EntryExpDeath()
{
    ChangeAnimation();
    CheckTime = 0.0f;
    DropItem();
}

void AImpCharacter::Patrol(float DeltaTime)
{
    if (FVector::DistXZ(GetActorLocation(), TurningLocations[CurTurningIndex]) < 10.0f)
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
        ChangeAnimation();

    }
    else
    {
        ChangeAnimation();

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

    CheckTime += DeltaTime;

    // 상태 머신 변경 메소드
    if (true == CheckActorInRange(GetWorld()->GetMainPawn())
        && CheckTime > CheckTimeLimit * 5.0f)
    {
        ChangeState(EEnemyState::ATTACK);
    }
}

void AImpCharacter::Attack(float DeltaTime)
{
    CheckTime += DeltaTime;

    if (CheckTimeLimit < CheckTime)
    {
        ChangeState(EEnemyState::TRACE);
    }
}


void AImpCharacter::Trace(float DeltaTime)
{
    FVector TargetLocation = GetWorld()->GetMainPawn()->GetActorLocation();

    FVector MoveDir = TargetLocation - GetActorLocation();
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

    CheckTime += DeltaTime;

    if (CheckTimeLimit < CheckTime)
    {
        ChangeState(EEnemyState::ATTACK);
    }
}

void AImpCharacter::Death(float DeltaTime)
{
    CheckTime += DeltaTime;

    if (CheckTime > CheckTimeLimit * 10.0f)
    {
        Destroy();
    }
}

void AImpCharacter::ExpDeath(float DeltaTime)
{
    CheckTime += DeltaTime;

    if (CheckTime > CheckTimeLimit * 10.0f)
    {
        Destroy();
    }
}

void AImpCharacter::ChangeAnimation()
{
    float DotDegree = FVector::GetVectorAngleDeg(GetCurDirection(), (GetWorld()->GetMainPawn()->GetActorLocation() - GetActorLocation()));
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
        AnimationName = "Attack_";
        break;
    case EEnemyState::DEATH:
        AnimationName = "Death_";
        break;
    case EEnemyState::EXP_DEATH:
        AnimationName = "ExpDeath_";
        break;
    default:
        break;
    }


    if (45.0f <= DotDegree && DotDegree < 135.0f)
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
    else if (135.0f <= DotDegree && DotDegree < 225.0f)
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

void AImpCharacter::TakeDamage(int Damage)
{
    AddHP(-Damage);

    if (0 > GetHP())
    {
        ChangeState(EEnemyState::DEATH);
    }
}

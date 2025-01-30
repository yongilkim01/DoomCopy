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
	SpriteComponent->CreateAnimation("Patrol_Forward", "DoomZombie.png", 0, 3, 0.5f, true);
	SpriteComponent->ChangeAnimation("Patrol_Forward");

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

    //MoveRight(-100.0f);

    UEngineDebug::OutPutString("Cur turning index : " + std::to_string(CurTurningIndex));

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
    float TestDistance = FVector::Dist(GetActorLocation(), TurningLocations[CurTurningIndex]);
    if (FVector::Dist(GetActorLocation(), TurningLocations[CurTurningIndex]) < 5.0f)
    {
        // ���� Ÿ�� ��ġ ����
        if (CurTurningIndex == TurningLocations.size() - 1)
        {
            CurTurningIndex = 0;
        }
        else
        {
            CurTurningIndex++;
        }

        // ���� �̵� ���� ������Ʈ
        SetCurDirection(GetDirectionToTargetLocation(TurningLocations[CurTurningIndex]));
        //ChangeMoveAnimation(GetCurDirection());  // �ִϸ��̼� ���� (�ʿ� ��)
    }
    else
    {
        FVector CurEnemyLocation = GetActorLocation();
        FVector TurningLocation = this->TurningLocations[CurTurningIndex];

        FVector MoveDir = TurningLocation - CurEnemyLocation;
        MoveDir.Normalize();

        // ���� �Ǵ� ����
        if (MoveDir.Z > 0)
        {
            MoveForward(Speed);  // ����
        }
        else
        {
            MoveForward(-Speed); // ����
        }

        // ���� �Ǵ� ���� �̵�
        if (MoveDir.X > 0)
        {
            MoveRight(Speed);  // ���� �̵�
        }
        else
        {
            MoveRight(-Speed); // ���� �̵�
        }
        
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

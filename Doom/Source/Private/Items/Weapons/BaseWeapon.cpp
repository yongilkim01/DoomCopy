#include "pch.h"
#include "Public/Items/Weapons/BaseWeapon.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>

ABaseWeapon::ABaseWeapon()
{
	std::shared_ptr<USceneComponent> SceneComponent = CreateDefaultSubObject<USceneComponent>();
	RootComponent = SceneComponent;

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
    //SpriteComponent->SetTexture("Shotgun_Idle.png");
    SpriteComponent->SetRelativeLocation({ 0.0f, 30.0f, 20.0f });
    SpriteComponent->SetRelativeScale3D({ 25.0f, 25.0f });
    SpriteComponent->SetAutoScale(false);
    
    SpriteComponent->CreateAnimation("ShotGun_Idle", "Shotgun.png", 0, 0, 0.1f, true);
    SpriteComponent->CreateAnimation("ShotGun_Fire", "Shotgun.png", 1, 5, 0.5f, false);

    SpriteComponent->SetAnimationEvent("ShotGun_Fire", 5, [this]() 
        {
            SpriteComponent->ChangeAnimation("ShotGun_Idle");
        });
   
    SpriteComponent->ChangeAnimation("ShotGun_Idle");

}

ABaseWeapon::~ABaseWeapon()
{

}

void ABaseWeapon::BeginPlay()
{
	AActor::BeginPlay();
}

void ABaseWeapon::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);

    //UEngineDebug::OutPutString("Weapon Location : " + GetActorLocation().ToString());

    BulletStartLocation = GetActorLocation() + FVector{0.0f, SpriteComponent->GetRelativeLocation().Y, GetActorForwardVector().Z * 10};


    if (true == bMoving)
    {
        // 시간 값 증가
        RunningTime += DeltaTime;

        // 사인파 계산
        float Amplitude = 0.01f;  // 진폭
        float Frequency = 1.0f;    // 주파수
        float DeltaWidth = Amplitude * FMath::Sin(2 * FMath::PI * Frequency * RunningTime);
        float DeltaHeight = -Amplitude * FMath::Sin(2 * FMath::PI * Frequency * RunningTime * 2.0f);

        AddActorLocation(FVector{ DeltaWidth, DeltaHeight, 0.0f });
    }
}

void ABaseWeapon::Reload()
{
    SpriteComponent->ChangeAnimation("ShotGun_Fire");
}

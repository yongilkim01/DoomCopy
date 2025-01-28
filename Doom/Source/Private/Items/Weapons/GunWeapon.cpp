#include "Doom.h"
#include "Public/Items/Weapons/GunWeapon.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>

AGunWeapon::AGunWeapon()
{
    SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
    SpriteComponent->SetupAttachment(RootComponent);
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

AGunWeapon::~AGunWeapon()
{

}

void AGunWeapon::BeginPlay()
{
    ABaseWeapon::BeginPlay();
}

void AGunWeapon::Tick(float DeltaTime)
{
    ABaseWeapon::Tick(DeltaTime);

    BulletStartLocation = GetActorLocation() + FVector{ 0.0f, SpriteComponent->GetRelativeLocation().Y, GetActorForwardVector().Z * 10 };

    if (true == bMoving)
    {
        RunningTime += DeltaTime;

        float Amplitude = 0.01f;  // ÁøÆø
        float Frequency = 1.0f;    // ÁÖÆÄ¼ö
        float DeltaWidth = Amplitude * FMath::Sin(2 * FMath::PI * Frequency * RunningTime);
        float DeltaHeight = -Amplitude * FMath::Sin(2 * FMath::PI * Frequency * RunningTime * 2.0f);

        AddActorLocation(FVector{ DeltaWidth, DeltaHeight, 0.0f });
    }
}

void AGunWeapon::Reload()
{
    SpriteComponent->ChangeAnimation("ShotGun_Fire");
}

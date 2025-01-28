#include "Doom.h"
#include "Public/Items/Weapons/ShotgunWeapon.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>

#include "Public/Items/Weapons/Bullet.h"

AShotgunWeapon::AShotgunWeapon()
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

AShotgunWeapon::~AShotgunWeapon()
{

}

void AShotgunWeapon::BeginPlay()
{
    AGunWeapon::BeginPlay();
}

void AShotgunWeapon::Tick(float DeltaTime)
{
    AGunWeapon::Tick(DeltaTime);

    BulletStartLocation = GetActorLocation() + FVector{ 0.0f, SpriteComponent->GetRelativeLocation().Y, GetActorForwardVector().Z * 10 };

}

void AShotgunWeapon::Fire()
{
    float BulletStartValue = -10.0f;

    for (int i = 0; i < 5; i++)
    {
        std::shared_ptr<ABullet> Bullet = GetWorld()->SpawnActor<ABullet>();
        FVector BulletLocation = GetActorLocation() + (GetActorForwardVector() * 50.0f) + (GetActorUpVector() * 30.0f) + (GetActorRightVector() * BulletStartValue);
        Bullet->SetActorLocation(BulletLocation);
        Bullet->SetBulletDirection(GetActorForwardVector());

        BulletStartValue += 5.0f;
    }


    Reload();
}

void AShotgunWeapon::Reload()
{
    SpriteComponent->ChangeAnimation("ShotGun_Fire");
}

#include "Doom.h"
#include "Public/Items/Weapons/PistolWeapon.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>

#include "Public/Items/Weapons/Bullet.h"

APistolWeapon::APistolWeapon()
{
    SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
    SpriteComponent->SetupAttachment(RootComponent);
    SpriteComponent->SetRelativeLocation({ 0.0f, 30.0f, 20.0f });
    SpriteComponent->SetRelativeScale3D({ 25.0f, 25.0f });
    SpriteComponent->SetAutoScale(false);

    SpriteComponent->CreateAnimation("Pistol_Idle", "Pistol.png", 0, 0, 0.1f, true);
    SpriteComponent->CreateAnimation("Pistol_Fire", "Pistol.png", 1, 4, 0.5f, false);

    SpriteComponent->SetAnimationEvent("Pistol_Fire", 4, [this]()
        {
            ChangeState(EGunState::MOVE);
        });

    SpriteComponent->ChangeAnimation("Pistol_Idle");
}

APistolWeapon::~APistolWeapon()
{

}

void APistolWeapon::BeginPlay()
{
    AGunWeapon::BeginPlay();

    GunMoveAmplitude = 0.001f;  // ÁøÆø
    GunMoveFrequency = 1.0f;    // ÁÖÆÄ¼ö
}

void APistolWeapon::Tick(float DeltaTime)
{
    AGunWeapon::Tick(DeltaTime);
}

void APistolWeapon::FireGun()
{
    if (EGunState::MOVE != CurGunState)
    {
        return;
    }

    ChangeState(EGunState::FIRE);
}

void APistolWeapon::EntryMove()
{
    SpriteComponent->ChangeAnimation("Pistol_Idle");
}

void APistolWeapon::EntryFire()
{
    std::shared_ptr<ABullet> Bullet = GetWorld()->SpawnActor<ABullet>();
    FVector BulletLocation = GetActorLocation() + (GetActorForwardVector() * 50.0f) + (GetActorUpVector() * 30.0f) + (GetActorRightVector());
    Bullet->SetActorLocation(BulletLocation);
    Bullet->SetBulletDirection(GetActorForwardVector());

}

void APistolWeapon::EntryReload()
{
    SpriteComponent->ChangeAnimation("Pistol_Fire");

}

void APistolWeapon::Move(float DeltaTime)
{
    AGunWeapon::Move(DeltaTime);

    BulletStartLocation = GetActorLocation() + FVector{ 0.0f, SpriteComponent->GetRelativeLocation().Y, GetActorForwardVector().Z * 10 };
}

void APistolWeapon::Fire(float DeltaTime)
{
    ChangeState(EGunState::RELOAD);
}

void APistolWeapon::Reload(float DeltaTime)
{
}
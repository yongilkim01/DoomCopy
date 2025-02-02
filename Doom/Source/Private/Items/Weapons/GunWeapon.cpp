#include "Doom.h"
#include "Public/Items/Weapons/GunWeapon.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>

#include "Public/Items/Weapons/Bullet.h"

AGunWeapon::AGunWeapon()
{

}

AGunWeapon::~AGunWeapon()
{

}

void AGunWeapon::BeginPlay()
{
    ABaseWeapon::BeginPlay();

    ChangeState(EGunState::MOVE);
}

void AGunWeapon::Tick(float DeltaTime)
{
    ABaseWeapon::Tick(DeltaTime);

    switch (CurGunState)
    {
    case EGunState::MOVE:
        Move(DeltaTime);
        break;
    case EGunState::FIRE:
        Fire(DeltaTime);
        break;
    case EGunState::RELOAD:
        Reload(DeltaTime);
        break;
    default:
        break;
    }

}

void AGunWeapon::FireGun()
{
    if (EGunState::MOVE != CurGunState)
    {
        return;
    }
}

void AGunWeapon::EntryMove()
{
}

void AGunWeapon::EntryFire()
{
}

void AGunWeapon::EntryReload()
{
}

void AGunWeapon::Move(float DeltaTime)
{
    if (true == bMoving)
    {
        RunningTime += DeltaTime;

        float DeltaWidth = GunMoveAmplitude * FMath::Sin(3 * FMath::PI * GunMoveFrequency * RunningTime);
        float DeltaHeight = -GunMoveAmplitude * FMath::Sin(3 * FMath::PI * GunMoveFrequency * RunningTime * 2.0f);

        AddActorLocation(FVector{ DeltaWidth, DeltaHeight, 0.0f });
    }
}

void AGunWeapon::Fire(float DeltaTime)
{

}

void AGunWeapon::Reload(float DeltaTime)
{

}

void AGunWeapon::ChangeState(EGunState GunState)
{
    CurGunState = GunState;

    switch (CurGunState)
    {
    case EGunState::MOVE:
        EntryMove();
        break;
    case EGunState::FIRE:
        EntryFire();
        break;
    case EGunState::RELOAD:
        EntryReload();
        break;
    default:
        break;
    }
}

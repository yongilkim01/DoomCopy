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
}

void AGunWeapon::Tick(float DeltaTime)
{
    ABaseWeapon::Tick(DeltaTime);

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

void AGunWeapon::Fire()
{

}

void AGunWeapon::Reload()
{

}

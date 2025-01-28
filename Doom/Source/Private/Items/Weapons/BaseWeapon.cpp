#include "Doom.h"
#include "Public/Items/Weapons/BaseWeapon.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>

ABaseWeapon::ABaseWeapon()
{
	std::shared_ptr<USceneComponent> SceneComponent = CreateDefaultSubObject<USceneComponent>();
	RootComponent = SceneComponent;
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
}
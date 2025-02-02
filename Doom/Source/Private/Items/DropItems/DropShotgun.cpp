#include "Doom.h"
#include "Public/Items/DropItems/DropShotgun.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include "Public/Global/GameInstance/DoomGameInstance.h"

#include "Public/Characters/DoomGuy.h"

ADropShotgun::ADropShotgun()
{
	RootComponent = CreateDefaultSubObject<UCapsuleComponent>();

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetRelativeScale3D({ 64.0f, 64.0f });
	SpriteComponent->SetAutoScale(false);
	SpriteComponent->OnBillboard();
	SpriteComponent->CreateAnimation("DropShotgun_Idle", "Weapons.png", 1, 1, 0.5f, true);
	SpriteComponent->ChangeAnimation("DropShotgun_Idle");

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetRelativeScale3D({ 15.0f, 15.0f, 15.0f });
	ShapeComponent->SetCollisionProfileName("DropItem");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);

	ShapeComponent->SetCollisionEnter([this](UShapeComponent* This, UShapeComponent* Other)
		{
			DoomGuyCharacter = dynamic_cast<ADoomGuyCharacter*>(Other);
			ActiveDropItem();

			This->GetOwner()->Destroy();
		});
}

ADropShotgun::~ADropShotgun()
{
}

void ADropShotgun::BeginPlay()
{
	ADropItem::BeginPlay();

}

void ADropShotgun::Tick(float DeltaTime)
{
	ADropItem::Tick(DeltaTime);

}

void ADropShotgun::EntryDrop()
{
}

void ADropShotgun::EntryPickup()
{
}

void ADropShotgun::Drop(float DeltaTime)
{
}

void ADropShotgun::Pickup(float DeltaTime)
{
}

void ADropShotgun::ActiveDropItem()
{
	GetGameInstance<UDoomGameInstance>()->AddDoomGuyWeaponCount(1);
}

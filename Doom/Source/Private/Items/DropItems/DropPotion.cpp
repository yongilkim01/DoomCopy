#include "Doom.h"
#include "Public/Items/DropItems/DropPotion.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include "Public/Global/GameInstance/DoomGameInstance.h"

ADropPotion::ADropPotion()
{
	RootComponent = CreateDefaultSubObject<UCapsuleComponent>();

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetRelativeScale3D({ 32.0f, 32.0f });
	SpriteComponent->SetAutoScale(false);
	SpriteComponent->OnBillboard();
	SpriteComponent->CreateAnimation("Portion_Idle", "Potion.png", 1, 1, 0.5f, true);
	SpriteComponent->ChangeAnimation("Portion_Idle");

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetRelativeScale3D({ 15.0f, 15.0f, 15.0f });
	ShapeComponent->SetCollisionProfileName("DropItem");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);

	ShapeComponent->SetCollisionEnter([this](UShapeComponent* This, UShapeComponent* Other)
		{
			This->GetOwner()->Destroy();
			GetGameInstance<UDoomGameInstance>()->AddDoomGuyHealth(1);
		});
}

ADropPotion::~ADropPotion()
{
}

void ADropPotion::BeginPlay()
{
	ADropItem::BeginPlay();

}

void ADropPotion::Tick(float DeltaTime)
{
	ADropItem::Tick(DeltaTime);

}

void ADropPotion::EntryDrop()
{
}

void ADropPotion::EntryPickup()
{
}

void ADropPotion::Drop(float DeltaTime)
{
}

void ADropPotion::Pickup(float DeltaTime)
{
}

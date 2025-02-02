#include "Doom.h"
#include "Public/Items/DropItems/DropArmor.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include "Public/Global/GameInstance/DoomGameInstance.h"

ADropArmor::ADropArmor()
{
	RootComponent = CreateDefaultSubObject<UCapsuleComponent>();

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetRelativeScale3D({ 64.0f, 64.0f });
	SpriteComponent->SetAutoScale(false);
	SpriteComponent->OnBillboard();
	SpriteComponent->CreateAnimation("Armor_Idle", "NormalArmor.png", 0, 0, 0.5f, true);
	SpriteComponent->ChangeAnimation("Armor_Idle");

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetRelativeScale3D({ 15.0f, 15.0f, 15.0f });
	ShapeComponent->SetCollisionProfileName("DropItem");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);

	ShapeComponent->SetCollisionEnter([this](UShapeComponent* This, UShapeComponent* Other)
		{
			This->GetOwner()->Destroy();
			GetGameInstance<UDoomGameInstance>()->SetDoomGuyArmor(100);
		});
}

ADropArmor::~ADropArmor()
{
}

void ADropArmor::BeginPlay()
{
	ADropItem::BeginPlay();

}

void ADropArmor::Tick(float DeltaTime)
{
	ADropItem::Tick(DeltaTime);

}

void ADropArmor::EntryDrop()
{
}

void ADropArmor::EntryPickup()
{
}

void ADropArmor::Drop(float DeltaTime)
{
}

void ADropArmor::Pickup(float DeltaTime)
{
}

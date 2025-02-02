#include "Doom.h"
#include "Public/Items/Environment/DrumContainer.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include "Public/Global/GameInstance/DoomGameInstance.h"

#include "Public/Characters/DoomGuy.h"

ADrumContainer::ADrumContainer()
{
	RootComponent = CreateDefaultSubObject<UCapsuleComponent>();

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetRelativeScale3D({ 128.0f, 128.0f });
	SpriteComponent->SetAutoScale(false);
	SpriteComponent->OnBillboard();
	SpriteComponent->CreateAnimation("Drum_Idle", "DrumContainer.png", 0, 2, 0.5f, true);
	SpriteComponent->CreateAnimation("Drum_Exp", "DrumContainer.png", 3, 6, 0.2f, false);
	SpriteComponent->SetAnimationEvent("Drum_Exp", 6, [this]() 
		{
			Destroy();
		});

	SpriteComponent->ChangeAnimation("Drum_Idle");

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetRelativeScale3D({ 20.0f, 20.0f, 20.0f });
	ShapeComponent->SetCollisionProfileName("EnvBody");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);

	AttackShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	AttackShapeComponent->SetupAttachment(RootComponent);
	AttackShapeComponent->SetRelativeScale3D({ 60.0f, 60.0f, 60.0f });
	AttackShapeComponent->SetCollisionProfileName("EnvAttack");
	AttackShapeComponent->SetCollisionType(ECollisionType::Sphere);
	AttackShapeComponent->SetCollisionEnter([this](UShapeComponent* This, UShapeComponent* Other)
		{
			This->GetOwner()->Destroy();
			GetGameInstance<UDoomGameInstance>()->AddDoomGuyHealth(-30);

		});
	AttackShapeComponent->SetActive(false);
}

ADrumContainer::~ADrumContainer()
{
}

void ADrumContainer::BeginPlay()
{
	AActor::BeginPlay();

}

void ADrumContainer::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

void ADrumContainer::Bomb()
{
	SpriteComponent->ChangeAnimation("Drum_Exp");
	AttackShapeComponent->SetActive(true);
}

#include "Doom.h"
#include "Public/Characters/Enemy/ImpCharacter.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

AImpCharacter::AImpCharacter()
{
	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetRelativeScale3D({ 120.0f, 120.0f });
	SpriteComponent->SetAutoScale(false);
	SpriteComponent->OnBillboard();
	SpriteComponent->CreateAnimation("Patrol_Forward", "DoomImp.png", 0, 3, 0.5f, true);
	SpriteComponent->ChangeAnimation("Patrol_Forward");

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetRelativeScale3D({ 30.0f, 30.0f, 30.0f });
	ShapeComponent->SetCollisionProfileName("EnemyBody");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);
}

AImpCharacter::~AImpCharacter()
{
}

void AImpCharacter::BeginPlay()
{
	AEnemyCharacter::BeginPlay();
}

void AImpCharacter::Tick(float DeltaTime)
{
	AEnemyCharacter::Tick(DeltaTime);
}

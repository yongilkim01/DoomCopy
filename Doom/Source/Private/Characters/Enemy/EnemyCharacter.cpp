#include "Doom.h"
#include "Public/Characters/Enemy/EnemyCharacter.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

UEnemyCharacter::UEnemyCharacter()
{
	std::shared_ptr<USceneComponent> SceneComponent = CreateDefaultSubObject<USceneComponent>();
	RootComponent = SceneComponent;

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetRelativeScale3D({ 120.0f, 120.0f });
	SpriteComponent->SetAutoScale(false);
	SpriteComponent->OnBillboard();
	SpriteComponent->CreateAnimation("Patrol_Forward", "DoomZombie.png", 0, 3, 0.5f, true);
	SpriteComponent->ChangeAnimation("Patrol_Forward");

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetRelativeScale3D({ 30.0f, 30.0f, 30.0f });
	ShapeComponent->SetCollisionProfileName("EnemyBody");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);

	//ShapeComponent->SetCollisionEnter([](UShapeComponent* _This, UShapeComponent* _Other)
	//	{
	//		//_Other->GetOwner()->Destroy();
	//		//// _Other->Destroy();
	//		//UEngineDebug::OutPutString("Enter");
	//	});
}

UEnemyCharacter::~UEnemyCharacter()
{
}

void UEnemyCharacter::BeginPlay()
{
	AActor::BeginPlay();
}

void UEnemyCharacter::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}


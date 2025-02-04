#include "Doom.h"
#include "Public/Items/Weapons/ImpFire.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>

#include <Engine/Classes/Engine/RenderAsset.h>

AImpFire::AImpFire()
{
	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	RootComponent = SpriteComponent;

	SpriteComponent->SetRelativeScale3D({ 128.0f, 128.0f });
	SpriteComponent->SetAutoScale(false);
	SpriteComponent->OnBillboard();
	SpriteComponent->CreateAnimation("Fire_Move", "DoomImpFire.png", 0, 1, 0.5f, true);
	SpriteComponent->CreateAnimation("Fire_Hit", "DoomImpFire.png", 2, 4, 0.2f, false);
	SpriteComponent->ChangeAnimation("Fire_Move");
	SpriteComponent->SetAnimationEvent("Fire_Hit", 4, [this]()
		{
			Destroy();
		});

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetCollisionProfileName("EnemyAttack");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);
	ShapeComponent->SetRelativeScale3D({ 3.0f, 3.0f, 3.0f });

	ShapeComponent->SetCollisionEnter([](UShapeComponent* This, UShapeComponent* Other)
		{
			//This->GetOwner()->Destroy();
			//Other->GetOwner()->Destroy();
			UEngineDebug::OutPutString("Player Hit!!");

		});

}

AImpFire::~AImpFire()
{
}

void AImpFire::BeginPlay()
{
	AActor::BeginPlay();
}

void AImpFire::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	CurLifeTime += DeltaTime;

	if (CurLifeTime > LifeTime)
	{
		Destroy();
	}

	if (Direction != FVector::ZERO)
	{
		SpriteComponent->AddForce(Direction * 2.0f);
	}
}

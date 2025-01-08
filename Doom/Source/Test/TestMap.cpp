#include "pch.h"
#include "TestMap.h"

#include "TestComponent.h"

#include <Classes/Components/PaperSpriteComponent.h>

ATestMap::ATestMap()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetTexture("Test.png");
}

ATestMap::~ATestMap()
{
}

void ATestMap::BeginPlay()
{
	AActor::BeginPlay();

}

void ATestMap::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}


#include "pch.h"
#include "NavMeshMap.h"

#include <Classes/Components/PaperSpriteComponent.h>

#include "Utils/NavMesh/Component/NavMeshComponent.h"

ANavMeshMap::ANavMeshMap()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	SpriteComponent = CreateDefaultSubObject<UNavMeshComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	//SpriteComponent->SetTexture("Test.png");
}

ANavMeshMap::~ANavMeshMap()
{
}

void ANavMeshMap::BeginPlay()
{
	AActor::BeginPlay();

}

void ANavMeshMap::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}


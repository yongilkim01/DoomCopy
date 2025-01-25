#include "Doom.h"
#include "Public/NavigationSystem/NavMeshMap.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>

#include <Engine/Classes/Components/StaticMeshComponent.h>

#include "Public/NavigationSystem/NavMeshComponent.h"

ANavMeshMap::ANavMeshMap()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	MapComponent = CreateDefaultSubObject<UStaticMeshComponent>();
	MapComponent->SetupAttachment(RootComponent);

	std::vector<int> GroundModelNumber;
	GroundModelNumber.reserve(20);

	GroundModelNumber.push_back(35);
	GroundModelNumber.push_back(36);
	GroundModelNumber.push_back(37);
	GroundModelNumber.push_back(41);
	GroundModelNumber.push_back(45);
	GroundModelNumber.push_back(51);
	GroundModelNumber.push_back(52);

	///MapComponent->SetModelDebug("doom_E1M1", GroundModelNumber);
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


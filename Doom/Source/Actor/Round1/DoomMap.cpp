#include "pch.h"
#include "DoomMap.h"

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/SceneComponent.h>
#include <Input/EngineInput.h>

#include "Componennts/DoomMapComponent.h"

ADoomMap::ADoomMap()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	DoomMapComponent = CreateDefaultSubObject<UDoomMapComponent>();

	//DoomMapComponent->SetRelativeScale3D({ 50, 50, 1.0f });
	DoomMapComponent->SetupAttachment(RootComponent);
}

ADoomMap::~ADoomMap()
{
}

void ADoomMap::BeginPlay()
{
	AActor::BeginPlay();
}

void ADoomMap::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

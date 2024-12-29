#include "pch.h"
#include "DrawSquareActor.h"

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/SceneComponent.h>
#include <Core/Misc/DirectoryHelper.h>
#include <Input/EngineInput.h>

#include "Componennts/DoomMapComponent.h"
#include "Componennts/LineComponent.h"
#include "Utils/Wad.h"

ADrawSquareActor::ADrawSquareActor()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	SquareComponent = CreateDefaultSubObject<UDoomMapComponent>();
	SquareComponent->SetupAttachment(RootComponent);
	SquareComponent->SetRelativeScale3D({ 50, 50, 1.0f });
}

ADrawSquareActor::~ADrawSquareActor()
{
}

void ADrawSquareActor::BeginPlay()
{
	AActor::BeginPlay();
}

void ADrawSquareActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	AddActorRotation(FVector{ 0.0f, 0.0f, 360 * DeltaTime });
}

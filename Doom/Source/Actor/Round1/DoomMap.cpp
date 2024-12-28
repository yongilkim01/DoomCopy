#include "pch.h"
#include "DoomMap.h"

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/SceneComponent.h>
#include <Input/EngineInput.h>

#include "Componennts/DoomMapComponent.h"
#include "Componennts/LineComponent.h"

ADoomMap::ADoomMap()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	DoomMapComponent = CreateDefaultSubObject<UDoomMapComponent>();
	DoomMapComponent->SetupAttachment(RootComponent);
	DoomMapComponent->SetRelativeScale3D({ 50, 50, 1.0f });


	LineComponent = CreateDefaultSubObject<ULineComponent>();
	LineComponent->SetupAttachment(RootComponent);
	LineComponent->SetRelativeScale3D({ 50, 50, 1.0f });
	LineComponent->SetWorldLocation({ 100.0f, 100.0f, 0.0f});
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

	if (UEngineInput::IsPress('A'))
	{
		AddActorRelativeLocation(FVector{ -100.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		AddActorRelativeLocation(FVector{ 100.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('W'))
	{
		AddActorRelativeLocation(FVector{ 0.0f, 100.0f * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		AddActorRelativeLocation(FVector{ 0.0f, -100.0f * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress('Q'))
	{
		AddActorRotation(FVector{ 0.0f, 0.0f , 360.0f * DeltaTime });
	}
}

#include "pch.h"
#include "DrawLineActor.h"

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/SceneComponent.h>
#include <Core/Misc/DirectoryHelper.h>
#include <Input/EngineInput.h>

#include "Componennts/LineComponent.h"
#include "Utils/Wad.h"

ADrawLineActor::ADrawLineActor()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	LineComponent = CreateDefaultSubObject<ULineComponent>();
	LineComponent->SetupAttachment(RootComponent);
	LineComponent->SetRelativeScale3D({ 50, 1.0f, 1.0f });
}

ADrawLineActor::~ADrawLineActor()
{
}

void ADrawLineActor::BeginPlay()
{
	AActor::BeginPlay();
}

void ADrawLineActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

#include "pch.h"
#include "NavMeshCharacter.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>
#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>

#include <Core/Misc/DirectoryHelper.h>

#include <Input/EngineInput.h>

ANavMeshCharacter::ANavMeshCharacter()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetTexture("Test2.png");
	// Renderer->SetSprite("Test.png", 2);
	// Renderer->SetAutoScale(true);
	// Renderer->SetAutoScaleRatio(5.0f);
	SpriteComponent->SetWorldScale3D({ 20.0f, 20.0f });

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetCollisionProfileName("Player");
	ShapeComponent->SetWorldScale3D({ 20.0f, 20.0f });
}

ANavMeshCharacter::~ANavMeshCharacter()
{
}

void ANavMeshCharacter::BeginPlay()
{
	AActor::BeginPlay();
}

void ANavMeshCharacter::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (UEngineInput::IsPress('A'))
	{
		AddActorRelativeLocation(FVector{ -Speed * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		AddActorRelativeLocation(FVector{ Speed * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('W'))
	{
		AddActorRelativeLocation(FVector{ 0.0f, 0.0f, Speed * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		AddActorRelativeLocation(FVector{ 0.0f, 0.0f, -Speed * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress(VK_UP))
	{
		AddActorRelativeLocation(FVector{ 0.0f, Speed * DeltaTime, 0.0f, 0.0f });

	}
	if (UEngineInput::IsPress(VK_DOWN))
	{
		AddActorRelativeLocation(FVector{ 0.0f, -Speed * DeltaTime, 0.0f, 0.0f });

	}
}


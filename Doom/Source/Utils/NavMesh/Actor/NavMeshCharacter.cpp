#include "pch.h"
#include "NavMeshCharacter.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>
#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>

#include <Core/Public/Misc/DirectoryHelper.h>

#include <Input/EngineInput.h>

ANavMeshCharacter::ANavMeshCharacter()
{
	//std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	//RootComponent = Default;

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetTexture("Test2.png");
	SpriteComponent->SetWorldScale3D({ 20.0f, 20.0f });
	SpriteComponent->SetEnableGravity(true);

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetCollisionProfileName("Player");
	ShapeComponent->SetWorldScale3D({ 20.0f, 20.0f });

	CurMouseLocation = UGameEngine::GetMainWindow().GetMousePos();
}

ANavMeshCharacter::~ANavMeshCharacter()
{
}

void ANavMeshCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = GetActorForwardVector();

		AddMovementInput(Direction, Value);
	}
}

void ANavMeshCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = GetActorRightVector();

		AddMovementInput(Direction, Value);
	}
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
		MoveRight(-Speed);
	}
	if (UEngineInput::IsPress('D'))
	{
		MoveRight(Speed);
	}
	if (UEngineInput::IsPress('W'))
	{
		MoveForward(Speed);
	}
	if (UEngineInput::IsPress('S'))
	{
		MoveForward(-Speed);
	}

	FVector PrevMouseLocation = CurMouseLocation;

	CurMouseLocation = UGameEngine::GetMainWindow().GetMousePos();

	AddActorRotation({ 0.0f, CurMouseLocation.X - PrevMouseLocation.X, 0.0f });
	
}


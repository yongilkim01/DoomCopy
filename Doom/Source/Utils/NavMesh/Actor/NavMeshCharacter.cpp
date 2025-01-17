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
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

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

void ANavMeshCharacter::BeginPlay()
{
	AActor::BeginPlay();
}

void ANavMeshCharacter::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (UEngineInput::IsPress('A'))
	{
		PerformMovement += FVector(-Speed * DeltaTime, 0.0f, 0.0f);
	}
	if (UEngineInput::IsPress('D'))
	{
		PerformMovement += FVector(Speed * DeltaTime, 0.0f, 0.0f);
	}
	if (UEngineInput::IsPress('W'))
	{
		PerformMovement += FVector(0.0f, 0.0f, Speed * DeltaTime);
	}
	if (UEngineInput::IsPress('S'))
	{
		PerformMovement += FVector(0.0f, 0.0f, -Speed * DeltaTime);
	}

	PerformMovement.Normalize();
	PerformMovement *= Speed * DeltaTime;

	FVector PrevMouseLocation = CurMouseLocation;

	CurMouseLocation = UGameEngine::GetMainWindow().GetMousePos();

	AddActorRotation({ 0.0f, CurMouseLocation.X - PrevMouseLocation.X, 0.0f });

	

	UEngineDebug::OutPutString("Mouse Location : " + CurMouseLocation.ToString());

	
}


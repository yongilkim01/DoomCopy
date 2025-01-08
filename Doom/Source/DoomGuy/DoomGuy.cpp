#include "pch.h"
#include "DoomGuy.h"

#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/StaticMeshComponent.h>
#include <Classes/Components/ShapeComponent.h>
#include <Classes/Camera/CameraActor.h>
#include <Classes/Camera/CameraComponent.h>

#include <Core/Misc/DirectoryHelper.h>

#include <Input/EngineInput.h>

ADoomGuy::ADoomGuy()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetTexture("Test.png");
	// Renderer->SetSprite("Test.png", 2);
	// Renderer->SetAutoScale(true);
	// Renderer->SetAutoScaleRatio(5.0f);
	SpriteComponent->SetWorldScale3D({ 30.0f, 30.0f });
	SpriteComponent->AddWorldLocation({ 0.0f, 5.0f, 0.0f });

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetCollisionProfileName("Player");
	ShapeComponent->SetWorldScale3D({ 30.0f, 30.0f });
	ShapeComponent->AddWorldLocation({ 0.0f, 5.0f, 0.0f });

	//ShapeComponent->SetCollisionEnter([](UShapeComponent* _This, UShapeComponent* _Other)
	//	{
	//		_Other->GetActor()->Destroy();
	//		// _Other->Destroy();
	//		UEngineDebug::OutPutString("Enter");
	//	});

}

ADoomGuy::~ADoomGuy()
{
}

void ADoomGuy::BeginPlay()
{
	AActor::BeginPlay();

	Camera = GetWorld()->GetMainCamera();

	PrevMouseLocation = Camera->ScreenMouseLocationToWorldLocation();
}

void ADoomGuy::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	FVector CurMouseLocation = Camera->ScreenMouseLocationToWorldLocation();

	UEngineDebug::OutPutString(Camera->GetMouseLocation().ToString());

	if (UEngineInput::IsPress('A'))
	{
		AddActorLocation(FVector{ -Speed * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		AddActorLocation(FVector{ Speed * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('W'))
	{
		AddActorLocation(FVector{ 0.0f, 0.0f, Speed * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		AddActorLocation(FVector{ 0.0f, 0.0f, -Speed * DeltaTime, 0.0f });
	}
}

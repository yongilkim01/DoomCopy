#include "pch.h"
#include "TestPlayer.h"

#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/StaticMeshComponent.h>
#include <Classes/Components/ShapeComponent.h>
#include <Classes/Camera/CameraActor.h>
#include <Classes/Camera/CameraComponent.h>

#include <Core/Misc/DirectoryHelper.h>

#include <Input/EngineInput.h>

ATestPlayer::ATestPlayer()
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

ATestPlayer::~ATestPlayer()
{
}

void ATestPlayer::BeginPlay()
{
	AActor::BeginPlay();
}

void ATestPlayer::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	
	DirectX::XMVECTOR OriginVector = DirectX::XMVectorSet(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z, 1.0f);

	DirectX::XMVECTOR Direction = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR Vector1 = DirectX::XMVectorSet(-150.0f, 0.0f, 150.0f, 1.0f);
	DirectX::XMVECTOR Vector2 = DirectX::XMVectorSet(150.0f, 0.0f, 150.0f, 1.0f);
	DirectX::XMVECTOR Vector3 = DirectX::XMVectorSet(0.0f, 0.0f, -150.0f, 1.0f);


	//Vector1 = DirectX::XMVectorSet(-141.78f, 51.2f, 150.0f, 1.0f);
	//Vector2 = DirectX::XMVectorSet(141.03f, -51.2f, 150.0f, 1.0f);
	//Vector3 = DirectX::XMVectorSet(0.0f, 0.0f, -150.0f, 1.0f);


	DirectX::TriangleTests::Intersects(OriginVector, Direction, Vector1, Vector2, Vector3, Result);

	if (9.0f < Result && Result < 11.0f)
	{
		//SetActorLocation({ 0.0f, 10.0f, 0.0f });
	}
	else
	{
		AddActorLocation({ 0.0f, -0.5f, 0.0f });
	}



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
}


#include "pch.h"
#include "TitleLogo.h"

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/SceneComponent.h>
#include <Input/EngineInput.h>

ATitleLogo::ATitleLogo()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	LogoRenderer = CreateDefaultSubObject<UPaperSpriteComponent>();
	LogoRenderer->CreateAnimation("Idle", "Player.png", 0, 70, 0.1f);
	LogoRenderer->ChangeAnimation("Idle");

	UPaperSpriteComponent::FrameAnimation* Animation = LogoRenderer->FindAnimation("Idle");
	Animation->IsAutoScale = true;
	Animation->AutoScaleRatio = 4.0f;

	LogoRenderer->SetRelativeScale3D({ 50, 50, 1.0f });
	LogoRenderer->SetupAttachment(RootComponent);
}

ATitleLogo::~ATitleLogo()
{
}

void ATitleLogo::BeginPlay()
{
	AActor::BeginPlay();
}

void ATitleLogo::Tick(float DeltaTime)
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
	if (UEngineInput::IsPress('E'))
	{
		Child->AddRelativeLocation(FVector{ 100.0f * DeltaTime, 0.0f , 0.0f });
	}
}

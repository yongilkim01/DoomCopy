#include "pch.h"
#include "TitleLogo.h"

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>
#include <Input/EngineInput.h>

ATitleLogo::ATitleLogo()
{
	LogoRenderer = CreateDefaultSubObject<UPaperSpriteComponent>();
	LogoRenderer->SetSprite("Player.png", 0);
	LogoRenderer->SetRelativeScale3D({ 50, 50, 1.0f });

	RootComponent = LogoRenderer;

	std::shared_ptr<UPaperSpriteComponent> Child = CreateDefaultSubObject<UPaperSpriteComponent>();
	Child->SetSprite("Player.png", 2);
	Child->SetWorldLocation({ 2.0f, 0.0f, 0.0f });
	Child->SetupAttachment(RootComponent);
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
		AddActorLocation(FVector{ -100.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		AddActorLocation(FVector{ 100.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('W'))
	{
		AddActorLocation(FVector{ 0.0f, 100.0f * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		AddActorLocation(FVector{ 0.0f, -100.0f * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress('Q'))
	{
		AddActorRotation(FVector{ 0.0f, 0.0f , 360.0f * DeltaTime });
	}
}

#include "pch.h"
#include "TitleLogo.h"

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>
#include <Input/EngineInput.h>

ATitleLogo::ATitleLogo()
{
	LogoRenderer = CreateDefaultSubObject<UPaperSpriteComponent>();
	LogoRenderer->SetSprite("Player.png", 0);
	RootComponent = LogoRenderer;

	SetActorRelativeScale3D({ 200.0f, 200.0f, 1.0f });
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
}

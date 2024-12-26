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
	LogoRenderer->SetSprite("Player.png", 0);
	LogoRenderer->SetRelativeScale3D({ 50, 50, 1.0f });
	LogoRenderer->SetupAttachment(RootComponent);
	//RootComponent = LogoRenderer;

	Child = CreateDefaultSubObject<UPaperSpriteComponent>();
	Child->SetSprite("Player.png", 2);
	Child->SetRelativeLocation({ 100.0f, 0.0f, 0.0f });
	Child->SetWorldScale3D({ 50.0f, 50.0f, 1.0f });
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

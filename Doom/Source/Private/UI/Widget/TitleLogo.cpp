#include "Doom.h"
#include "Public/UI/Widget/TitleLogo.h"

#include <Core/Public/Math/EngineMath.h>
#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/SceneComponent.h>
#include <Input/EngineInput.h>

ATitleLogo::ATitleLogo()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	// 랜더러를 만든다.
	LogoRenderer = CreateDefaultSubObject<UPaperSpriteComponent>();
	LogoRenderer->SetupAttachment(RootComponent);
	LogoRenderer->SetAutoScaleRatio(5.0f);

	LogoRenderer->CreateAnimation("Idle", "Tevi", 0, 3, 0.5f);
	LogoRenderer->CreateAnimation("Move", "Tevi", 4, 16, 0.3f);
	LogoRenderer->ChangeAnimation("Move");
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
		LogoRenderer->ChangeAnimation("Move");

		//Child->AddRelativeLocation(FVector{ 100.0f * DeltaTime, 0.0f , 0.0f });
	}
}

#include "pch.h"
#include "TitleLogo.h"

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>

ATitleLogo::ATitleLogo()
{
	LogoRenderer = CreateDefaultSubObject<UPaperSpriteComponent>();
	LogoRenderer->SetSprite("Player.png", 0);

	SetActorRelativeScale3D({ 600.0f, 600.0f, 1.0f });
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

	//AddActorLocation(FVector{ 0.001f, 0.0f, 0.0f });
}

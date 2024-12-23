#include "pch.h"
#include "TitleLogo.h"

#include <Renderer/Renderer.h>
#include <Core/Math/EngineMath.h>


ATitleLogo::ATitleLogo()
{
	LogoRenderer = CreateDefaultSubObject<URenderer>();

	SetActorRelativeScale3D({ 200.0f, 100.0f, 1.0f });
	SetActorLocation({ 200.0f, 100.0f, 1.0f });
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

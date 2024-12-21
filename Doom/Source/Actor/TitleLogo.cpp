#include "pch.h"
#include "TitleLogo.h"

#include <Renderer/Renderer.h>
#include <Math/EngineMath.h>

ATitleLogo::ATitleLogo()
{
	LogoRenderer = CreateDefaultSubObject<URenderer>();

	SetActorRelativeScale3D({ 100.0f, 100.0f, 1.0f });
	//SetActorRela
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
}

#include "pch.h"
#include "TitleLogo.h"

#include <Renderer/Renderer.h>

ATitleLogo::ATitleLogo()
{
	std::shared_ptr<URenderer> Renderer = CreateDefaultSubObject<URenderer>();
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

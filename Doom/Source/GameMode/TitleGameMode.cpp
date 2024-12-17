#include "pch.h"
#include "TitleGameMode.h"
#include "Actor/TitleLogo.h"

ATitleGameMode::ATitleGameMode()
{
	GetWorld()->SpawnActor<ATitleLogo>();
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

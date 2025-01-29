#include "Engine.h"
#include "Engine/Classes/GameFramework/GameMode.h"

AGameMode::AGameMode()
{
}

AGameMode::~AGameMode()
{
}

void AGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

void AGameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}
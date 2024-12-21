#include "pch.h"
#include "TitleGameMode.h"
#include "Actor/TitleLogo.h"

#include <Classes/Camera/CameraActor.h>

ATitleGameMode::ATitleGameMode()
{
	GetWorld()->SpawnActor<ATitleLogo>();

	std::shared_ptr<ACameraActor> CameraActor = GetWorld()->GetMainCamera();
	CameraActor->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

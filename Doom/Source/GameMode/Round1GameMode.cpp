#include "pch.h"
#include "Round1GameMode.h"
#include "Actor/Round1/DoomMap.h"

#include <Classes/Camera/CameraActor.h>


ARound1GameMode::ARound1GameMode()
{
	DoomMap = GetWorld()->SpawnActor<ADoomMap>();
	//DoomMap->GetRen

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
}

ARound1GameMode::~ARound1GameMode()
{
}

void ARound1GameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);


}
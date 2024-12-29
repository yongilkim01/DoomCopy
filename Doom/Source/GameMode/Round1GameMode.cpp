#include "pch.h"
#include "Round1GameMode.h"

#include "Actor/Round1/DoomMap.h"
#include "Utils/DrawLineActor.h"
#include "Utils/DrawSquareActor.h"

#include <Classes/Camera/CameraActor.h>


ARound1GameMode::ARound1GameMode()
{
	{
		std::shared_ptr<ADrawSquareActor> DrawSquareActor = GetWorld()->SpawnActor<ADrawSquareActor>();
		DrawSquareActorVector.push_back(DrawSquareActor);
;	}
	{
		std::shared_ptr<ADrawLineActor> DrawLineActor = GetWorld()->SpawnActor<ADrawLineActor>();
		DrawLineActor->SetActorRotation(FVector{ 0.0f, 0.0f, 30.0f });
		DrawLineActor->SetActorRelativeScale3D(FVector{ 1000.0f, 1.0f, 1.0f });
		DrawLineActorVector.push_back(DrawLineActor);
	}

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
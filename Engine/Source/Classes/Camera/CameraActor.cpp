#include "pch.h"
#include "CameraActor.h"
#include "Classes/Camera/CameraComponent.h"

ACameraActor::ACameraActor()
{
	CameraComponent = CreateDefaultSubObject<UCameraComponent>();
}

ACameraActor::~ACameraActor()
{
}

void ACameraActor::BeginPlay()
{
	AActor::BeginPlay();
}

void ACameraActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	CameraComponent->CalculateViewAndProjection();

}

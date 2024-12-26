#include "pch.h"
#include "CameraActor.h"
#include "Classes/Camera/CameraComponent.h"

ACameraActor::ACameraActor()
{
	CameraComponent = CreateDefaultSubObject<UCameraComponent>();
	RootComponent = CameraComponent;
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

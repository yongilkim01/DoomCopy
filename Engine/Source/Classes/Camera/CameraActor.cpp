#include "pch.h"
#include "CameraActor.h"
#include "Classes/Camera/CameraComponent.h"
#include "Core/EngineCore.h"
#include "Platform/Window.h"

ACameraActor::ACameraActor()
{
	CameraComponent = CreateDefaultSubObject<UCameraComponent>();
	RootComponent = CameraComponent;
}

ACameraActor::~ACameraActor()
{
	CameraComponent = nullptr;
}

FVector ACameraActor::ScreenMouseLocationToWorldLocation()
{
	FVector Size = UEngineCore::GetMainWindow().GetWindowSize();

	FVector MousePos = UEngineCore::GetMainWindow().GetMousePos();

	float4x4 Mat;

	Mat.ViewPort(Size.X, Size.Y, 0.0f, 0.0f, 0.0f, 1.0f);

	FTransform CameraTransform = GetActorLoaction();

	MousePos = MousePos * Mat.InverseReturn();
	MousePos = MousePos * CameraTransform.Projection.InverseReturn();
	MousePos = MousePos * CameraTransform.View.InverseReturn();

	return MousePos;
}

FVector ACameraActor::ScreenMouseLocationToWorldLocationWithOutLocation()
{
	return FVector();
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

#include "pch.h"
#include "TestComponent.h"

#include "Classes/Camera/CameraComponent.h"
#include "Classes/Engine/Texture.h"
#include "Classes/Engine/StaticMesh.h"

UTestComponent::UTestComponent()
{
	CreateRenderUnit();
	SetMesh("Rect");
	SetMaterial("SpriteMaterial");
}

UTestComponent::~UTestComponent()
{
}

void UTestComponent::BeginPlay()
{
	UPrimitiveComponent::BeginPlay();
}

void UTestComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UPrimitiveComponent::Render(CameraComponent, DeltaTime);
}
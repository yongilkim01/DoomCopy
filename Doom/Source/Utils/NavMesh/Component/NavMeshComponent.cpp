#include "pch.h"
#include "NavMeshComponent.h"

#include "Classes/Camera/CameraComponent.h"
#include "Classes/Engine/Texture.h"
#include "Classes/Engine/StaticMesh.h"

UNavMeshComponent::UNavMeshComponent()
{
	CreateRenderUnit();
	SetMesh("Triangle");
	SetMaterial("NavMeshTestMaterial");

	//MyColor.Albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);

	//GetRenderUnit().ConstantBufferLinkData("FMyColor", MyColor);
}

UNavMeshComponent::~UNavMeshComponent()
{
}

void UNavMeshComponent::BeginPlay()
{
	UPrimitiveComponent::BeginPlay();
}

void UNavMeshComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UPrimitiveComponent::Render(CameraComponent, DeltaTime);
}
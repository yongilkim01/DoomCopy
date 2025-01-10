#include "pch.h"
#include "NavMeshComponent.h"

#include "Classes/Camera/CameraComponent.h"
#include "Classes/Engine/Texture.h"
#include "Classes/Engine/StaticMesh.h"

#include "Utils/NavMesh/Actor/NavMeshResource.h"

UNavMeshComponent::UNavMeshComponent()
{
	NavMesh = UStaticMesh::Find<UNavMeshResource>("NavMapResource");

	for (int i = 0; i < NavMesh->GetNavMeshResourceCount(); i++)
	{
		CreateRenderUnit();
		SetMesh("NavMapResource" + std::to_string(i));
		SetMaterial("CollisionDebugMaterial");
	}

	//MyColor.Albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);

	//GetRenderUnit().ConstantBufferLinkData("FMyColor", MyColor);
}

UNavMeshComponent::~UNavMeshComponent()
{
}

std::vector<EngineVertex>& UNavMeshComponent::GetVertexVector()
{
	return NavMesh->GetVertexVector();
}

std::vector<unsigned int>& UNavMeshComponent::GetIndexVector()
{
	return NavMesh->GetIndexVector();
}

void UNavMeshComponent::BeginPlay()
{
	UPrimitiveComponent::BeginPlay();
}

void UNavMeshComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UPrimitiveComponent::Render(CameraComponent, DeltaTime);
}
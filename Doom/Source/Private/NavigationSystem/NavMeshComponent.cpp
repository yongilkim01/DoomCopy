#include "Doom.h"
#include "Public/NavigationSystem/NavMeshComponent.h"

#include <NavigationSystem/Public/NavigationSystem.h>

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Engine/Texture.h"
#include "Engine/Classes/Engine/StaticMesh.h"

#include "Public/NavigationSystem/NavMeshResource.h"

UNavMeshComponent::UNavMeshComponent()
{
	NavMesh = UNavMeshResource::Find<UNavMeshResource>("NavMapResource");

	for (int i = 0; i < NavMesh->GetNavMeshResourceCount(); i++)
	{
		for (int j = 0; j < UNavigationSystem::GetInstance().GetGroundMeshIndexVector().size(); j++)
		{
			if (UNavigationSystem::GetInstance().GetGroundMeshIndexVector()[j] == i)
			{
				CreateRenderUnit();
				SetMesh("NavMapResource" + std::to_string(i));
				SetMaterial("CollisionDebugMaterial");
			}
		}
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
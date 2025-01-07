#include "pch.h"
#include "StaticMeshComponent.h"

#include "Classes/Camera/CameraComponent.h"
#include "Classes/Engine/Texture.h"
#include "Classes/Engine/StaticMesh.h"

UStaticMeshComponent::UStaticMeshComponent()
{
}

UStaticMeshComponent::~UStaticMeshComponent()
{
}

void UStaticMeshComponent::BeginPlay()
{
	UPrimitiveComponent::BeginPlay();
}

void UStaticMeshComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UPrimitiveComponent::Render(CameraComponent, DeltaTime);
}

void UStaticMeshComponent::SetModel(std::string_view ModelName)
{
	StaticMesh = UStaticMesh::Find<UStaticMesh>(ModelName);

	for (int i = 0; i < StaticMesh->GetStaticMeshCount(); i++)
	{
			CreateRenderUnit();
			SetMesh(StaticMesh->GetMeshNameByIndex(i), MeshCount);
			SetMaterial(StaticMesh->GetMeshNameByIndex(i), MeshCount);

			if (nullptr != StaticMesh->GetTextureByIndex(i))
			{
				GetRenderUnit(MeshCount).SetTexture("ImageTexture", StaticMesh->GetTextureNameByIndex(i));
			}
			MeshCount++;
	}
}
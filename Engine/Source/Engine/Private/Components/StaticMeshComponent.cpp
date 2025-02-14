#include "Engine.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Engine/Texture.h"
#include "Engine/Classes/Engine/StaticMesh.h"

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
		if (i == 1  ||
			i == 5  ||
			i == 12 ||
			i == 16 ||
			i == 19 ||
			i == 20 ||
			i == 23 || 
			i == 25 ||
			i == 35 || 
			i == 36 ||
			i == 37 || 
			i == 41 ||
			i == 45 ||
			i == 51 ||
			i == 52)
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
}

void UStaticMeshComponent::SetModel(std::string_view ModelName, std::vector<int>& ModelNumber)
{
	StaticMesh = UStaticMesh::Find<UStaticMesh>(ModelName);

	for (int i = 0; i < StaticMesh->GetStaticMeshCount(); i++)
	{
		for (int j = 0; j < ModelNumber.size(); j++)
		{
			if (i == ModelNumber[j])
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
	}
}

void UStaticMeshComponent::SetModelDebug(std::string_view ModelName, std::vector<int>& ModelNumber)
{
	StaticMesh = UStaticMesh::Find<UStaticMesh>(ModelName);

	for (int i = 0; i < StaticMesh->GetStaticMeshCount(); i++)
	{
		for (int j = 0; j < ModelNumber.size(); j++)
		{
			if (i == ModelNumber[j])
			{
				CreateRenderUnit();
				SetMesh(StaticMesh->GetMeshNameByIndex(i), MeshCount);
				SetMaterial("CollisionDebugMaterial", MeshCount);

				MeshCount++;
			}
		}
	}
}

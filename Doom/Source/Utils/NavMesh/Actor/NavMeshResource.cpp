#include "pch.h"
#include "NavMeshResource.h"

#include <Classes/Engine/Mesh.h>
#include <Classes/Engine/Texture.h>

#include <Core/Materials/Material.h>
#include <Core/Misc/DirectoryHelper.h>

#include <Rendering/TextureLoader.h>
#include <Rendering/AiMesh.h>
#include <Rendering/Buffer/EngineVertex.h>
#include <Rendering/Buffer/VertexBuffer.h>
#include <Rendering/Buffer/IndexBuffer.h>

UNavMeshResource::UNavMeshResource()
{
}

UNavMeshResource::~UNavMeshResource()
{
}

std::shared_ptr<UNavMeshResource> UNavMeshResource::Create(std::string_view ModelName)
{
	std::string UpperName = ToUpperName(ModelName);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드된 스테틱 메시입니다 " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UNavMeshResource> NewAsset = std::make_shared<UNavMeshResource>();
	AddAsset<UNavMeshResource>(NewAsset, ModelName, "");
	NewAsset->CreateAsset(ModelName);

	return NewAsset;
}


std::string UNavMeshResource::GetMeshNameByIndex(int Index)
{
	return NavMeshResourceDataVector[Index].Mesh->GetName();
}

std::string UNavMeshResource::GetTextureNameByIndex(int Index)
{
	return NavMeshResourceDataVector[Index].Texture->GetName();
}

std::shared_ptr<UTexture> UNavMeshResource::GetTextureByIndex(int Index)
{
	return NavMeshResourceDataVector[Index].Texture;
}

std::shared_ptr<UMesh> UNavMeshResource::GetMeshByIndex(int Index)
{
	return NavMeshResourceDataVector[Index].Mesh;
}

void UNavMeshResource::CreateAsset(std::string_view ModelName)
{
	LoadModel(ModelName);
}


bool UNavMeshResource::LoadModel(std::string_view ModelName)
{
	EngineVertex Vertex1 = EngineVertex{ FVector(-100.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
	EngineVertex Vertex2 = EngineVertex{ FVector(100.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 1.0f, 0.0f, 1.0f} };
	EngineVertex Vertex3 = EngineVertex{ FVector(-100.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex4 = EngineVertex{ FVector(100.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f} };
	EngineVertex Vertex5 = EngineVertex{ FVector(-170.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex6 = EngineVertex{ FVector(-170.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex7 = EngineVertex{ FVector(-240.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex8 = EngineVertex{ FVector(-240.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };


	Vertexs.push_back(Vertex1);
	Vertexs.push_back(Vertex2);
	Vertexs.push_back(Vertex3);
	Vertexs.push_back(Vertex4);
	Vertexs.push_back(Vertex5);
	Vertexs.push_back(Vertex6);
	Vertexs.push_back(Vertex7);
	Vertexs.push_back(Vertex8);

	Indexs.push_back(0);
	Indexs.push_back(1);
	Indexs.push_back(2);

	Indexs.push_back(1);
	Indexs.push_back(3);
	Indexs.push_back(2);

	Indexs.push_back(0);
	Indexs.push_back(2);
	Indexs.push_back(4);

	Indexs.push_back(5);
	Indexs.push_back(0);
	Indexs.push_back(4);
	
	Indexs.push_back(6);
	Indexs.push_back(5);
	Indexs.push_back(4);

	Indexs.push_back(6);
	Indexs.push_back(4);
	Indexs.push_back(7);

	FVertexBuffer::Create(ModelName.data() + std::to_string(NavMeshResourceDataVector.size()), Vertexs);
	FIndexBuffer::Create(ModelName.data() + std::to_string(NavMeshResourceDataVector.size()), Indexs);

	FNavMeshResourceData NavMeshResourceData;

	NavMeshResourceData.Mesh = UMesh::Create(ModelName.data() + std::to_string(NavMeshResourceDataVector.size()));

	NavMeshResourceDataVector.push_back(NavMeshResourceData);

	return true;
}
#include "pch.h"
#include "NavMeshResource.h"

#include <Engine/Classes/Engine/Mesh.h>
#include <Engine/Classes/Engine/Texture.h>
#include <Engine/Classes/Engine/GameEngine.h>
#include <Engine/Public/Materials/Material.h>
#include <Core/Public/Serialization/Archive.h>
#include <Core/Public/Misc/DirectoryHelper.h>
#include <Core/Public/Misc/FileHelper.h>

#include <Rendering/Buffer/EngineVertex.h>
#include <Rendering/Buffer/VertexBuffer.h>
#include <Rendering/Buffer/IndexBuffer.h>

#include <NavigationSystem/Public/NavigationSystem.h>

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
	FDirectoryHelper Dir;

	// "Resources" 디렉토리를 찾음
	if (false == Dir.MoveParentToDirectory("Resources", "Doom"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
	}

	Dir.Append("Data\\Test2.MapData");

	if (true == Dir.IsExists())
	{
		UNavigationSystem::GetInstance().SetLoadFileExist(true);
		std::string FilePath = Dir.GetPathToString();

		FFileHelper MapDataFile = Dir.GetFile(FilePath);
		FArchive Ser;

		MapDataFile.FileOpen("rb");

		MapDataFile.Read(Ser);

		// NaviDataVector 초기화
		UNavigationSystem::GetInstance().GetNaviDataVector().clear();

		// NaviDataVector 크기 읽기
		int NaviDataCount = 0;
		Ser >> NaviDataCount;

		for (int i = 0; i < NaviDataCount; ++i)
		{
			FNaviData NaviData;

			NaviData.DataVectorIndex = i;
			// DataVectorIndex 읽기
			//Ser >> NaviData.DataVectorIndex;

			// IndexArray 읽기
			for (int j = 0; j < 3; j++)
			{
				int IndexValue = 0;
				Ser >> IndexValue;
				NaviData.IndexArray[j] = IndexValue;
			}

			// LinkNaviDataIndex 읽기
			int LinkCount = 0;
			Ser >> LinkCount;
			NaviData.LinkNaviDataIndex.resize(LinkCount);
			for (int& LinkIndex : NaviData.LinkNaviDataIndex)
			{
				Ser >> LinkIndex;
			}

			// VertexDataVector 읽기
			int VertexCount = 0;
			Ser >> VertexCount;
			NaviData.VertexDataVector.resize(VertexCount);

			for (EngineVertex& Vertex : NaviData.VertexDataVector)
			{
				Ser >> Vertex.POSITION;
				Ser >> Vertex.TEXCOORD;
				Ser >> Vertex.COLOR;
			}

			// NaviDataVector에 추가
			UNavigationSystem::GetInstance().GetNaviDataVector().push_back(NaviData);
		}
	}

	LoadModel(ModelName);
}


bool UNavMeshResource::LoadModel(std::string_view ModelName)
{
	EngineVertex Vertex1 = EngineVertex{ FVector(-100.0f, 50.0f, 100.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
	EngineVertex Vertex2 = EngineVertex{ FVector(100.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 1.0f, 0.0f, 1.0f} };
	EngineVertex Vertex3 = EngineVertex{ FVector(-100.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex4 = EngineVertex{ FVector(100.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f} };
	EngineVertex Vertex5 = EngineVertex{ FVector(-170.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex6 = EngineVertex{ FVector(-170.0f, 60.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex7 = EngineVertex{ FVector(-240.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex8 = EngineVertex{ FVector(-240.0f, 50.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };


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
	
	
	if(false == UNavigationSystem::GetInstance().IsLoadFileExist())
	{
		UNavigationSystem::GetInstance().CreateNaviData(Vertexs, Indexs);
	}
	

	FVertexBuffer::Create(ModelName.data() + std::to_string(NavMeshResourceDataVector.size()), Vertexs);
	FIndexBuffer::Create(ModelName.data() + std::to_string(NavMeshResourceDataVector.size()), Indexs);

	FNavMeshResourceData NavMeshResourceData;

	NavMeshResourceData.Mesh = UMesh::Create(ModelName.data() + std::to_string(NavMeshResourceDataVector.size()));

	NavMeshResourceDataVector.push_back(NavMeshResourceData);

	return true;
}
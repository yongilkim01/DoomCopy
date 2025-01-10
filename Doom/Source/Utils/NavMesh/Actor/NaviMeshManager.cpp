#include "pch.h"
#include "NaviMeshManager.h"

#include <GameFramework/Actor.h>
#include <Rendering/Buffer/EngineVertex.h>

UNaviMeshManager::UNaviMeshManager()
{
}

UNaviMeshManager::~UNaviMeshManager()
{
}

void UNaviMeshManager::LoadModel(std::string_view ModelPath)
{
	std::vector<EngineVertex> VertexDataVector;
	std::vector<unsigned int> IndexDataVector;

	EngineVertex Vertex1 = EngineVertex{ FVector(-100.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
	EngineVertex Vertex2 = EngineVertex{ FVector(100.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 1.0f, 0.0f, 1.0f} };
	EngineVertex Vertex3 = EngineVertex{ FVector(-100.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex4 = EngineVertex{ FVector(100.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f} };
	EngineVertex Vertex5 = EngineVertex{ FVector(-170.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex6 = EngineVertex{ FVector(-170.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };


	VertexDataVector.push_back(Vertex1);
	VertexDataVector.push_back(Vertex2);
	VertexDataVector.push_back(Vertex3);
	VertexDataVector.push_back(Vertex4);
	VertexDataVector.push_back(Vertex5);
	VertexDataVector.push_back(Vertex6);

	{
		FNaviData NaviData;

		NaviData.DataVectorIndex = NaviDataVector.size();
		NaviData.VertexDataVector.reserve(3);

		for (int i = 0; i < 3; i++)
		{
			IndexDataVector.push_back(i);
			NaviData.IndexArray[i] = IndexDataVector[IndexDataVector.size() - 1];

			NaviData.VertexDataVector.push_back(VertexDataVector[NaviData.IndexArray[i]]);
		}

		NaviDataVector.push_back(NaviData);
	}
	//{
	//	IndexDataVector.push_back(1);
	//	IndexDataVector.push_back(3);
	//	IndexDataVector.push_back(2);

	//	FNaviData NaviData;

	//	NaviData.DataVectorIndex = NaviDataVector.size();
	//	NaviData.IndexArray[0] = IndexDataVector[IndexDataVector.size() - 1];
	//	NaviData.IndexArray[1] = IndexDataVector[IndexDataVector.size() - 1];
	//	NaviData.IndexArray[2] = IndexDataVector[IndexDataVector.size() - 1];

	//	NaviDataVector.push_back(NaviData);
	//}
	//{
	//	IndexDataVector.push_back(0);
	//	IndexDataVector.push_back(2);
	//	IndexDataVector.push_back(4);

	//	FNaviData NaviData;

	//	NaviData.DataVectorIndex = NaviDataVector.size();
	//	NaviData.IndexArray[0] = IndexDataVector[IndexDataVector.size() - 1];
	//	NaviData.IndexArray[1] = IndexDataVector[IndexDataVector.size() - 1];
	//	NaviData.IndexArray[2] = IndexDataVector[IndexDataVector.size() - 1];

	//	NaviDataVector.push_back(NaviData);
	//}
	//{
	//	IndexDataVector.push_back(5);
	//	IndexDataVector.push_back(0);
	//	IndexDataVector.push_back(4);

	//	FNaviData NaviData;

	//	NaviData.DataVectorIndex = NaviDataVector.size();
	//	NaviData.IndexArray[0] = IndexDataVector[IndexDataVector.size() - 1];
	//	NaviData.IndexArray[1] = IndexDataVector[IndexDataVector.size() - 1];
	//	NaviData.IndexArray[2] = IndexDataVector[IndexDataVector.size() - 1];

	//	NaviDataVector.push_back(NaviData);
	//}

	int Size = NaviDataVector.size();
}

void UNaviMeshManager::LinkNaviData()
{
}

bool FNaviData::Intersect(AActor* PlayerCharacter, AActor* MapActor)
{
	DirectX::XMVECTOR OriginVector = DirectX::XMVectorSet(PlayerCharacter->GetActorLocation().X, PlayerCharacter->GetActorLocation().Y, PlayerCharacter->GetActorLocation().Z, 1.0f);
	DirectX::XMVECTOR Direction = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	FTransform TestMapTransform = MapActor->GetActorTransform();

	DirectX::XMVECTOR Vector1 = (VertexDataVector[0].POSITION * TestMapTransform.ScaleMat * TestMapTransform.RotationMat * TestMapTransform.LocationMat).DirectVector;
	DirectX::XMVECTOR Vector2 = (VertexDataVector[1].POSITION * TestMapTransform.ScaleMat * TestMapTransform.RotationMat * TestMapTransform.LocationMat).DirectVector;
	DirectX::XMVECTOR Vector3 = (VertexDataVector[2].POSITION * TestMapTransform.ScaleMat * TestMapTransform.RotationMat * TestMapTransform.LocationMat).DirectVector;

	//DirectX::XMVECTOR Vector1 = DirectX::XMVectorSet(-150.0f, 0.0f, 150.0f, 1.0f);
	//DirectX::XMVECTOR Vector2 = DirectX::XMVectorSet(150.0f, 0.0f, 150.0f, 1.0f);
	//DirectX::XMVECTOR Vector3 = DirectX::XMVectorSet(0.0f, 0.0f, -150.0f, 1.0f);

	float Distance = 0.0f;

	DirectX::TriangleTests::Intersects(OriginVector, Direction, Vector1, Vector2, Vector3, Distance);

	if (9.0f < Distance && Distance < 11.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

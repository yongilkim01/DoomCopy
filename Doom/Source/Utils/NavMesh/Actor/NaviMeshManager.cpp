#include "pch.h"
#include "NaviMeshManager.h"

#include <GameFramework/Actor.h>
#include <Rendering/Buffer/EngineVertex.h>

FVector UNaviMeshManager::GetLocationPlayerNaviData(int Index)
{
	FTransform TestMapTransform = MapActor->GetActorTransform();

	return NaviDataVector[0].VertexDataVector[Index].POSITION * TestMapTransform.ScaleMat * TestMapTransform.RotationMat * TestMapTransform.LocationMat;
}

UNaviMeshManager::UNaviMeshManager()
{
}

UNaviMeshManager::~UNaviMeshManager()
{
}

void UNaviMeshManager::CreateNaviData(std::vector<EngineVertex>& VertexVector, std::vector<unsigned int>& IndexVector)
{
	if (0 != IndexVector.size() % 3)
	{
		MSGASSERT("인덱스 버퍼의 크기가 3개로 떨어지지 않습니다");
		return;
	}

	for (int i = 0; i < IndexVector.size(); i += 3)
	{
		FNaviData NaviData;

		NaviData.DataVectorIndex = NaviDataVector.size();
		NaviData.VertexDataVector.reserve(3);
		
		NaviData.IndexArray[0] = IndexVector[i];
		NaviData.IndexArray[1] = IndexVector[i + 1];
		NaviData.IndexArray[2] = IndexVector[i + 2];

		for (int VertexIndex = 0; VertexIndex < 3; VertexIndex++)
		{
			NaviData.VertexDataVector.push_back(VertexVector[NaviData.IndexArray[VertexIndex]]);
		}

		NaviDataVector.push_back(NaviData);
	}
}

void UNaviMeshManager::Init(AActor* InPlayerActor, AActor* InMapActor, std::string_view ModelPath)
{
	PlayerActor = InPlayerActor;
	MapActor = InMapActor;

	//LoadModel(ModelPath);

	CheckPlayerNaviDataInit();
	LinkNaviData();
}

void UNaviMeshManager::LoadModel(std::string_view ModelPath)
{
	std::vector<EngineVertex> VertexDataVector;
	std::vector<unsigned int> IndexDataVector;

	EngineVertex Vertex1 = EngineVertex{ FVector(-100.0f, 50.0f, 100.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
	EngineVertex Vertex2 = EngineVertex{ FVector(100.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 1.0f, 0.0f, 1.0f} };
	EngineVertex Vertex3 = EngineVertex{ FVector(-100.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex4 = EngineVertex{ FVector(100.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f} };
	EngineVertex Vertex5 = EngineVertex{ FVector(-170.0f, 0.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex6 = EngineVertex{ FVector(-170.0f, 60.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex7 = EngineVertex{ FVector(-240.0f, 0.0f, 100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };
	EngineVertex Vertex8 = EngineVertex{ FVector(-240.0f, 50.0f, -100.0f), {0.0f , 0.0f }, {0.0f, 0.0f, 1.0f, 1.0f} };

	VertexDataVector.push_back(Vertex1);
	VertexDataVector.push_back(Vertex2);
	VertexDataVector.push_back(Vertex3);
	VertexDataVector.push_back(Vertex4);
	VertexDataVector.push_back(Vertex5);
	VertexDataVector.push_back(Vertex6);
	VertexDataVector.push_back(Vertex7);
	VertexDataVector.push_back(Vertex8);

	IndexDataVector.push_back(0);
	IndexDataVector.push_back(1);
	IndexDataVector.push_back(2);

	IndexDataVector.push_back(1);
	IndexDataVector.push_back(3);
	IndexDataVector.push_back(2);

	IndexDataVector.push_back(0);
	IndexDataVector.push_back(2);
	IndexDataVector.push_back(4);

	IndexDataVector.push_back(5);
	IndexDataVector.push_back(0);
	IndexDataVector.push_back(4);

	IndexDataVector.push_back(6);
	IndexDataVector.push_back(5);
	IndexDataVector.push_back(4);

	IndexDataVector.push_back(6);
	IndexDataVector.push_back(4);
	IndexDataVector.push_back(7);

	CreateNaviData(VertexDataVector, IndexDataVector);

	int Size = NaviDataVector.size();
}

void UNaviMeshManager::LinkNaviData()
{
	for (int i = 0; i < NaviDataVector.size(); i++) {
		for (int j = 0; j < NaviDataVector.size(); j++) {
			if (i == j) {
				continue; // 자기 자신은 비교하지 않음
			}

			// IndexArray 비교
			bool hasOverlap = false;
			for (int i2 = 0; i2 < 3; i2++) {
				for (int j2 = 0; j2 < 3; j2++) {
					if (NaviDataVector[i].IndexArray[i2] == NaviDataVector[j].IndexArray[j2]) {
						hasOverlap = true; // 겹치는 값 발견
						break;
					}
				}
				if (hasOverlap) break;
			}

			// 겹치는 경우 LinkNaviDataIndex에 추가
			if (hasOverlap) {
				NaviDataVector[i].LinkNaviDataIndex.push_back(j);
			}
		}
	}
}

void UNaviMeshManager::CheckPlayerNaviDataInit()
{
	for (int i = 0; i < NaviDataVector.size(); i++)
	{
		float Result = NaviDataVector[i].Intersect(PlayerActor, MapActor);

		if (0.0f != Result)
		{
			Distance = Result;
			CurrentPlayerNaviDataIndex = i;
			break;
		}
	}
}

void UNaviMeshManager::CheckPlayerNaviDataTick()
{
	for (int i = 0; i < NaviDataVector[CurrentPlayerNaviDataIndex].LinkNaviDataIndex.size(); i++)
	{
		int CheckIndex = NaviDataVector[CurrentPlayerNaviDataIndex].LinkNaviDataIndex[i];
		float Result = 0.0f;
		Result = NaviDataVector[CheckIndex].Intersect(PlayerActor, MapActor);

		if (0.0f != Result)
		{
			Distance = Result;
			CurrentPlayerNaviDataIndex = CheckIndex;
			break;
		}
	}
}

void UNaviMeshManager::Tick(float DeltaTime)
{
	CheckPlayerNaviDataTick();

	float DistancePlayerToMap = NaviDataVector[CurrentPlayerNaviDataIndex].Intersect(PlayerActor, MapActor);
	UEngineDebug::OutPutString("Distance player to map : " + std::to_string(DistancePlayerToMap));

	if (DistancePlayerToMap != 0)
	{
		PlayerActor->SetActorLocation({ PlayerActor->GetActorLocation().X, PlayerActor->GetActorLocation().Y - DistancePlayerToMap + 10.0f, PlayerActor->GetActorLocation().Z });
	}
	else
	{
		PlayerActor->AddActorLocation({ 0.0f, -0.09f, 0.0f });

	}
}

float FNaviData::Intersect(AActor* PlayerCharacter, AActor* MapActor)
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

	float Result = 0.0f;

	DirectX::TriangleTests::Intersects(OriginVector, Direction, Vector1, Vector2, Vector3, Result);

	//UNaviMeshManager::GetInstance().Distance = Distance;

	return Result;
}

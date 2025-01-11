#include "pch.h"
#include "NavigationSystem/Public/NavigationSystem.h"

#include <GameFramework/Actor.h>
#include <Rendering/Buffer/EngineVertex.h>

FVector UNavigationSystem::GetLocationPlayerNaviData(int Index)
{
	FTransform TestMapTransform = MapActor->GetActorTransform();

	return NaviDataVector[0].VertexDataVector[Index].POSITION * TestMapTransform.ScaleMat * TestMapTransform.RotationMat * TestMapTransform.LocationMat;
}

UNavigationSystem::UNavigationSystem()
{
}

UNavigationSystem::~UNavigationSystem()
{
}

void UNavigationSystem::CreateNaviData(std::vector<EngineVertex>& VertexVector, std::vector<unsigned int>& IndexVector)
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

void UNavigationSystem::Init(AActor* InPlayerActor, AActor* InMapActor, std::string_view ModelPath)
{
	PlayerActor = InPlayerActor;
	MapActor = InMapActor;

	CheckPlayerNaviDataInit();

	if (false == IsLoadFileExist())
	{
		LinkNaviData();
	}
}

void UNavigationSystem::LinkNaviData()
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

void UNavigationSystem::CheckPlayerNaviDataInit()
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

void UNavigationSystem::CheckPlayerNaviDataTick()
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

void UNavigationSystem::Tick(float DeltaTime)
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

	float Result = 0.0f;

	DirectX::TriangleTests::Intersects(OriginVector, Direction, Vector1, Vector2, Vector3, Result);

	return Result;
}

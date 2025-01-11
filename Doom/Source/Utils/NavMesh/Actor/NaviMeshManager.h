#pragma once
#include <Rendering/Buffer/EngineVertex.h>

class AActor;
/**
 *	설명
 */
struct FNaviData
{
	int DataVectorIndex = -1;
	int IndexArray[3] = { -1, -1, -1 };
	std::vector<int> LinkNaviDataIndex;
	std::vector<EngineVertex> VertexDataVector;
	float Intersect(AActor* PlayerCharacter, AActor* MapActor);
};

/**
 *	설명
 */
class UNaviMeshManager
{
public:
	static UNaviMeshManager& GetInstance()
	{
		static UNaviMeshManager Inst = UNaviMeshManager();
		return Inst;
	}
	/** 소멸자 */
	~UNaviMeshManager();

	/** 객체 값 복사 방지 */
	UNaviMeshManager(const UNaviMeshManager& Other) = delete;
	UNaviMeshManager(UNaviMeshManager&& Other) noexcept = delete;
	UNaviMeshManager& operator=(const UNaviMeshManager& Other) = delete;
	UNaviMeshManager& operator=(UNaviMeshManager&& Other) noexcept = delete;

	void CreateNaviData(std::vector<EngineVertex>& VertexVector, std::vector<unsigned int>& IndexVector);
	void Init(AActor* InPlayerActor, AActor* InMapActor, std::string_view Modelpath);
	void LoadModel(std::string_view ModelPath);
	void LinkNaviData();
	void CheckPlayerNaviDataInit();
	void CheckPlayerNaviDataTick();
	void Tick(float DeltaTime);

	/** 겟, 셋 메소드 */
	int GetCurNaviDataIndex()
	{
		return CurrentPlayerNaviDataIndex;
	}
	std::vector<FNaviData>& GetNaviDataVector()
	{
		return NaviDataVector;
	}
	FNaviData& GetNaviDataByIndex(int Index)
	{
		return NaviDataVector[Index];
	}
	FNaviData& GetPlayerNaviData()
	{
		if (-1 == CurrentPlayerNaviDataIndex)
		{
			MSGASSERT("현재 플레이어의 네비 데이터가 존재하지 않습니다");
		}
		
		return NaviDataVector[CurrentPlayerNaviDataIndex];
	}
	FVector GetLocationPlayerNaviData(int Index);

	float Distance = 0.0f;

protected:

private:
	/** 생성자 */
	UNaviMeshManager();

	std::vector<FNaviData> NaviDataVector;

	int CurrentPlayerNaviDataIndex = -1;

	AActor* PlayerActor = nullptr;
	AActor* MapActor = nullptr;
};


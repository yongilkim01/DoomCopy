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
class UNavigationSystem
{
public:
	static UNavigationSystem& GetInstance()
	{
		static UNavigationSystem Inst = UNavigationSystem();
		return Inst;
	}
	/** 소멸자 */
	~UNavigationSystem();

	/** 객체 값 복사 방지 */
	UNavigationSystem(const UNavigationSystem& Other) = delete;
	UNavigationSystem(UNavigationSystem&& Other) noexcept = delete;
	UNavigationSystem& operator=(const UNavigationSystem& Other) = delete;
	UNavigationSystem& operator=(UNavigationSystem&& Other) noexcept = delete;

	void CreateNaviData(std::vector<EngineVertex>& VertexVector, std::vector<unsigned int>& IndexVector);
	void Init(AActor* InPlayerActor, AActor* InMapActor, std::string_view Modelpath);
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
	bool IsLoadFileExist()
	{
		return bLoadedFile;
	}
	void SetLoadFileExist(bool Value)
	{
		bLoadedFile = Value;
	}

	float Distance = 0.0f;

protected:

private:
	/** 생성자 */
	UNavigationSystem();

	std::vector<FNaviData> NaviDataVector;

	int CurrentPlayerNaviDataIndex = -1;

	AActor* PlayerActor = nullptr;
	AActor* MapActor = nullptr;

	bool bLoadedFile = false;
};


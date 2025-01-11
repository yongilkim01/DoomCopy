#pragma once
#include <Rendering/Buffer/EngineVertex.h>

class AActor;
/**
 *	����
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
 *	����
 */
class UNavigationSystem
{
public:
	static UNavigationSystem& GetInstance()
	{
		static UNavigationSystem Inst = UNavigationSystem();
		return Inst;
	}
	/** �Ҹ��� */
	~UNavigationSystem();

	/** ��ü �� ���� ���� */
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

	/** ��, �� �޼ҵ� */
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
			MSGASSERT("���� �÷��̾��� �׺� �����Ͱ� �������� �ʽ��ϴ�");
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
	/** ������ */
	UNavigationSystem();

	std::vector<FNaviData> NaviDataVector;

	int CurrentPlayerNaviDataIndex = -1;

	AActor* PlayerActor = nullptr;
	AActor* MapActor = nullptr;

	bool bLoadedFile = false;
};


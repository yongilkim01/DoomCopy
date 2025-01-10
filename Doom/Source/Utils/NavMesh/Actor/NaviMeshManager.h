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
	bool Intersect(AActor* PlayerCharacter, AActor* MapActor);
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

	void LoadModel(std::string_view ModelPath);
	void LinkNaviData();
	std::vector<FNaviData>& GetNaviDataVector()
	{
		return NaviDataVector;
	}

protected:

private:
	/** 생성자 */
	UNaviMeshManager();

	std::vector<FNaviData> NaviDataVector;

};


#pragma once

/**
 *	설명
 */
struct FNaviData
{
	int DataVectorIndex = -1;
	int IndexArray[3] = { -1, -1, -1 };
	std::vector<int> LinkNaviDataIndex;

	void Intersect(AActor* Player, float& Dis)
	{
		// DirectX::TriangleTests::Intersects()
	}
};

/**
 *	설명
 */
class UNaviMeshManager
{
public:
	/** 생성자, 소멸자 */
	UNaviMeshManager();
	~UNaviMeshManager();

	/** 객체 값 복사 방지 */
	UNaviMeshManager(const UNaviMeshManager& Other) = delete;
	UNaviMeshManager(UNaviMeshManager&& Other) noexcept = delete;
	UNaviMeshManager& operator=(const UNaviMeshManager& Other) = delete;
	UNaviMeshManager& operator=(UNaviMeshManager&& Other) noexcept = delete;

	void LoadModel(std::string_view ModelPath);
	void LinkNaviData();

protected:

private:
	std::vector<FNaviData> NaviDataVector;

};


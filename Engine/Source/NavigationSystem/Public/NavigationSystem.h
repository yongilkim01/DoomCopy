#pragma once
#include <Rendering/Buffer/EngineVertex.h>

class AActor;

class AiMesh;
class UMesh;
class UTexture;

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiTexture;
struct VERTEX;
struct TEXTURE;

enum aiTextureType;


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
	float Intersect(FVector CheckVector, AActor* MapActor);
};

/**
 *	설명
 */
class UNavigationSystem
{
public:
	ENGINE_API static UNavigationSystem& GetInstance();

	/** 소멸자 */
	ENGINE_API ~UNavigationSystem();

	/** 객체 값 복사 방지 */
	UNavigationSystem(const UNavigationSystem& Other) = delete;
	UNavigationSystem(UNavigationSystem&& Other) noexcept = delete;
	UNavigationSystem& operator=(const UNavigationSystem& Other) = delete;
	UNavigationSystem& operator=(UNavigationSystem&& Other) noexcept = delete;

	ENGINE_API void CreateNaviData(std::vector<EngineVertex>& VertexVector, std::vector<unsigned int>& IndexVector);
	ENGINE_API void CreateNaviData(std::string_view DirectoryName, std::string_view ObjFileName, std::vector<int>& InGroundMeshIndexVector);
	ENGINE_API void Init(AActor* InPlayerActor, AActor* InMapActor, std::string_view Modelpath);
	ENGINE_API void LinkNaviData();
	ENGINE_API void CheckPlayerNaviDataInit();
	ENGINE_API void CheckPlayerNaviDataTick();
	ENGINE_API bool CheckGroundMesh();
	ENGINE_API bool CheckDataFileExist();
	ENGINE_API void Tick(float DeltaTime);
	ENGINE_API float DistanceToActor(AActor* Actor);
	ENGINE_API float DistanceToVector(FVector Location);
	ENGINE_API void CheckVectorNaviData(FVector Location);


	bool LoadModel(std::string_view LoadObjPath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadFromDataFile();

	/** 겟, 셋 메소드 */
	ENGINE_API int GetCurNaviDataIndex()
	{
		return CurrentPlayerNaviDataIndex;
	}
	ENGINE_API std::vector<FNaviData>& GetNaviDataVector()
	{
		return NaviDataVector;
	}
	ENGINE_API FNaviData& GetNaviDataByIndex(int Index)
	{
		return NaviDataVector[Index];
	}
	ENGINE_API FNaviData& GetPlayerNaviData()
	{
		if (-1 == CurrentPlayerNaviDataIndex)
		{
			MSGASSERT("현재 플레이어의 네비 데이터가 존재하지 않습니다");
		}

		return NaviDataVector[CurrentPlayerNaviDataIndex];
	}
	ENGINE_API FVector GetLocationPlayerNaviData(int Index);
	ENGINE_API bool IsLoadFileExist()
	{
		return bLoadedFile;
	}
	ENGINE_API void SetLoadFileExist(bool Value)
	{
		bLoadedFile = Value;
	}
	ENGINE_API float GetDistance()
	{
		return Distance;
	}
	ENGINE_API void SetDistance(float NewValue)
	{
		Distance = NewValue;
	}
	ENGINE_API std::vector<int>& GetGroundMeshIndexVector()
	{
		return GroundMeshIndexVector;
	}
	ENGINE_API float GetCheckDistance()
	{
		return CheckDistance;
	}
	ENGINE_API void SetCheckDistance(float Value)
	{
		CheckDistance = Value;
	}

protected:

private:
	/** 생성자 */
	ENGINE_API UNavigationSystem();

	std::vector<FNaviData> NaviDataVector;

	int CurrentPlayerNaviDataIndex = -1;

	AActor* PlayerActor = nullptr;
	AActor* MapActor = nullptr;

	bool bLoadedFile = false;
	float Distance = 0.0f;

	std::string DirectoryPath = "";
	std::vector<int> GroundMeshIndexVector;

	int DataCount = 0;

	float CheckDistance = 110.0f;
};


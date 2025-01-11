#include "pch.h"
#include "NavigationSystem/Public/NavigationSystem.h"

#include <Core/Misc/DirectoryHelper.h>
#include <Core/Misc/FileHelper.h>

#include <GameFramework/Actor.h>
#include <Rendering/Buffer/EngineVertex.h>

#include "Rendering/TextureLoader.h"
#include "Rendering/AiMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> 
#include <assimp/scene.h>

FVector UNavigationSystem::GetLocationPlayerNaviData(int Index)
{
	FTransform TestMapTransform = MapActor->GetActorTransform();

	return NaviDataVector[0].VertexDataVector[Index].POSITION * TestMapTransform.ScaleMat * TestMapTransform.RotationMat * TestMapTransform.LocationMat;
}

UNavigationSystem::UNavigationSystem()
{
}

UNavigationSystem& UNavigationSystem::GetInstance()
{
	static UNavigationSystem Inst = UNavigationSystem();
	return Inst;
}

UNavigationSystem::~UNavigationSystem()
{
}

void UNavigationSystem::CreateNaviData(std::string_view DirectoryName, std::string_view ObjFileName)
{
	FDirectoryHelper DirectoryHelper;

	if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
		return;
	}

	DirectoryHelper.Append("Models\\");
	DirectoryHelper.Append(DirectoryName);

	std::string Path = DirectoryHelper.GetPathToString();
	std::string ObjPath = Path + "\\" + ObjFileName.data() + +".obj";
	std::string MtlPath = Path + "\\" + ObjFileName.data() + +".mtl";

	if (true == CheckDataFileExist())
	{
		LoadFromDataFile();
	}
	else
	{
		LoadModel(ObjPath);
	}

	size_t Size = NaviDataVector.size();
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

bool UNavigationSystem::CheckDataFileExist()
{
	FDirectoryHelper Dir;

	// "Resources" 디렉토리를 찾음
	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
	}

	Dir.Append("Data\\Test2.MapData");

	return Dir.IsExists();
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


bool UNavigationSystem::LoadModel(std::string_view LoadObjPath)
{
	std::string FileName = LoadObjPath.data();
	Assimp::Importer AssimpImporter;

	const aiScene* pScene = AssimpImporter.ReadFile(FileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
	{
		return false;
	}

	DirectoryPath = FileName.substr(0, FileName.find_last_of("/\\"));

	ProcessNode(pScene->mRootNode, pScene);

	return true;
}

void UNavigationSystem::ProcessNode(aiNode* AssimpNode, const aiScene* AssimpScene)
{
	for (UINT i = 0; i < AssimpNode->mNumMeshes; i++)
	{
		aiMesh* AssimpMesh = AssimpScene->mMeshes[AssimpNode->mMeshes[i]];

		ProcessMesh(AssimpMesh, AssimpScene);
	}

	for (UINT i = 0; i < AssimpNode->mNumChildren; i++)
	{
		ProcessNode(AssimpNode->mChildren[i], AssimpScene);
	}
}

void UNavigationSystem::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<EngineVertex> Vertexs;
	std::vector<unsigned int> Indexs;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		EngineVertex Vertex = EngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f} };

		Vertex.POSITION.X = mesh->mVertices[i].x;
		Vertex.POSITION.Y = mesh->mVertices[i].y;
		Vertex.POSITION.Z = mesh->mVertices[i].z;
		Vertex.POSITION.W = 1.0f;

		Vertexs.push_back(Vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			Indexs.push_back(face.mIndices[j]);
		}
	}

	CreateNaviData(Vertexs, Indexs);
}

void UNavigationSystem::LoadFromDataFile()
{
	FDirectoryHelper Dir;

	// "Resources" 디렉토리를 찾음
	if (false == Dir.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
	}

	Dir.Append("Data\\Test2.MapData");

	SetLoadFileExist(true);

	std::string FilePath = Dir.GetPathToString();

	FFileHelper MapDataFile = Dir.GetFile(FilePath);
	FArchive Ser;

	MapDataFile.FileOpen("rb");

	MapDataFile.Read(Ser);

	// NaviDataVector 초기화
	GetNaviDataVector().clear();

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
		GetNaviDataVector().push_back(NaviData);
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

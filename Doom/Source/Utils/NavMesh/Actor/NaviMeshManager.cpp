#include "pch.h"
#include "NaviMeshManager.h"

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
		IndexDataVector.push_back(0);
		IndexDataVector.push_back(1);
		IndexDataVector.push_back(2);

		FNaviData NaviData;

		NaviData.DataVectorIndex = NaviDataVector.size();
		NaviData.IndexArray[0] = IndexDataVector[IndexDataVector.size() - 1];
		NaviData.IndexArray[1] = IndexDataVector[IndexDataVector.size() - 1];
		NaviData.IndexArray[2] = IndexDataVector[IndexDataVector.size() - 1];

		NaviDataVector.push_back(NaviData);
	}
	{
		IndexDataVector.push_back(1);
		IndexDataVector.push_back(3);
		IndexDataVector.push_back(2);

		FNaviData NaviData;

		NaviData.DataVectorIndex = NaviDataVector.size();
		NaviData.IndexArray[0] = IndexDataVector[IndexDataVector.size() - 1];
		NaviData.IndexArray[1] = IndexDataVector[IndexDataVector.size() - 1];
		NaviData.IndexArray[2] = IndexDataVector[IndexDataVector.size() - 1];

		NaviDataVector.push_back(NaviData);
	}
	{
		IndexDataVector.push_back(0);
		IndexDataVector.push_back(2);
		IndexDataVector.push_back(4);

		FNaviData NaviData;

		NaviData.DataVectorIndex = NaviDataVector.size();
		NaviData.IndexArray[0] = IndexDataVector[IndexDataVector.size() - 1];
		NaviData.IndexArray[1] = IndexDataVector[IndexDataVector.size() - 1];
		NaviData.IndexArray[2] = IndexDataVector[IndexDataVector.size() - 1];

		NaviDataVector.push_back(NaviData);
	}
	{
		IndexDataVector.push_back(5);
		IndexDataVector.push_back(0);
		IndexDataVector.push_back(4);

		FNaviData NaviData;

		NaviData.DataVectorIndex = NaviDataVector.size();
		NaviData.IndexArray[0] = IndexDataVector[IndexDataVector.size() - 1];
		NaviData.IndexArray[1] = IndexDataVector[IndexDataVector.size() - 1];
		NaviData.IndexArray[2] = IndexDataVector[IndexDataVector.size() - 1];

		NaviDataVector.push_back(NaviData);
	}

	int Size = NaviDataVector.size();
}

void UNaviMeshManager::LinkNaviData()
{
}


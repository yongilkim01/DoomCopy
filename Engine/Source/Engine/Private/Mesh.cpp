#include "pch.h"
#include "Engine/Classes/Engine/Mesh.h"

UMesh::UMesh()
{
}

UMesh::~UMesh()
{
}

std::shared_ptr<UMesh> UMesh::Create(std::string_view Name, std::string_view VertexBuffer, std::string_view IndexBuffer)
{
	std::string UpperName = ToUpperName(Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드된 에셋입니다. " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UMesh> NewAsset = std::make_shared<UMesh>();
	AddAsset<UMesh>(NewAsset, Name, "");
	NewAsset->VertexBuffer = FVertexBuffer::Find<FVertexBuffer>(VertexBuffer);
	NewAsset->IndexBuffer = FIndexBuffer::Find<FIndexBuffer>(IndexBuffer);

	return NewAsset;
}

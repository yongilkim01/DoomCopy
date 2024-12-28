#include "pch.h"
#include "StaticMesh.h"

UStaticMesh::UStaticMesh()
{
}

UStaticMesh::~UStaticMesh()
{
}

std::shared_ptr<UStaticMesh> UStaticMesh::Create(std::string_view Name, std::string_view VertexBuffer, std::string_view IndexBuffer)
{
	std::string UpperName = ToUpperName(Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드된 에셋입니다. " + UpperName);
		return nullptr;
	}
	std::shared_ptr<UStaticMesh> NewAsset = std::make_shared<UStaticMesh>();
	AddAsset<UStaticMesh>(NewAsset, Name, "");
	NewAsset->VertexBuffer = FVertexBuffer::Find<FVertexBuffer>(VertexBuffer);
	NewAsset->IndexBuffer = FIndexBuffer::Find<FIndexBuffer>(IndexBuffer);

	return NewAsset;
}

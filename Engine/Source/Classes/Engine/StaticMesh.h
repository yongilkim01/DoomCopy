#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"

/**
 *	����
 */
class UStaticMesh : public URenderAsset
{
public:
	/** ������, �Ҹ��� */
	UStaticMesh();
	~UStaticMesh();

	/** ��ü �� ���� ���� */
	UStaticMesh(const UStaticMesh& Other) = delete;
	UStaticMesh(UStaticMesh&& Other) noexcept = delete;
	UStaticMesh& operator=(const UStaticMesh& Other) = delete;
	UStaticMesh& operator=(UStaticMesh&& Other) noexcept = delete;

	ENGINE_API static std::shared_ptr<UStaticMesh> Create(std::string_view Name)
	{
		return Create(Name, Name, Name);
	}
	ENGINE_API static std::shared_ptr<UStaticMesh> Create(std::string_view Name, std::string_view VertexBuffer, std::string_view IndexBuffer);

	/** ��, �� �޼ҵ� */
	std::shared_ptr<FVertexBuffer> GetVertexBuffer()
	{
		return VertexBuffer;
	}
	std::shared_ptr<FIndexBuffer> GetIndexBuffer()
	{
		return IndexBuffer;
	}

protected:

private:
	std::shared_ptr<FVertexBuffer> VertexBuffer;
	std::shared_ptr<FIndexBuffer> IndexBuffer;

};


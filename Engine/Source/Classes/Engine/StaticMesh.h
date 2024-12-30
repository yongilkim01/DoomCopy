#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"

/**
 *	설명
 */
class UStaticMesh : public URenderAsset
{
public:
	/** 생성자, 소멸자 */
	UStaticMesh();
	~UStaticMesh();

	/** 객체 값 복사 방지 */
	UStaticMesh(const UStaticMesh& Other) = delete;
	UStaticMesh(UStaticMesh&& Other) noexcept = delete;
	UStaticMesh& operator=(const UStaticMesh& Other) = delete;
	UStaticMesh& operator=(UStaticMesh&& Other) noexcept = delete;

	ENGINE_API static std::shared_ptr<UStaticMesh> Create(std::string_view Name)
	{
		return Create(Name, Name, Name);
	}
	ENGINE_API static std::shared_ptr<UStaticMesh> Create(std::string_view Name, std::string_view VertexBuffer, std::string_view IndexBuffer);

	/** 겟, 셋 메소드 */
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


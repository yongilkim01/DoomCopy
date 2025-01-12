#pragma once
#include "Engine/Classes/Engine/RenderAsset.h"
#include "Rendering/Buffer/IndexBuffer.h"
#include "Rendering/Buffer/VertexBuffer.h"

struct EVertexData
{
	float4 POSITION;
	float4 TEXCOORD;
	float4 COLOR;
};

/**
 *	설명
 */
class UMesh : public URenderAsset
{
public:
	/** 생성자, 소멸자 */
	UMesh();
	~UMesh();

	/** 객체 값 복사 방지 */
	UMesh(const UMesh& Other) = delete;
	UMesh(UMesh&& Other) noexcept = delete;
	UMesh& operator=(const UMesh& Other) = delete;
	UMesh& operator=(UMesh&& Other) noexcept = delete;

	ENGINE_API static std::shared_ptr<UMesh> Create(std::string_view Name)
	{
		return Create(Name, Name, Name);
	}
	ENGINE_API static std::shared_ptr<UMesh> Create(std::string_view Name, std::string_view VertexBuffer, std::string_view IndexBuffer);

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


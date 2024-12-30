#pragma once
#include "Classes/Engine/RenderAsset.h"

/**
 *	설명
 */
class FVertexBuffer : public URenderAsset
{
public:
	/** 생성자, 소멸자 */
	FVertexBuffer();
	~FVertexBuffer();

	/** 객체 값 복사 방지 */
	FVertexBuffer(const FVertexBuffer& Other) = delete;
	FVertexBuffer(FVertexBuffer&& Other) noexcept = delete;
	FVertexBuffer& operator=(const FVertexBuffer& Other) = delete;
	FVertexBuffer& operator=(FVertexBuffer&& Other) noexcept = delete;

	template<typename VertexDataType>
	static std::shared_ptr<FVertexBuffer> Create(std::string_view Name, 
												 const std::vector<VertexDataType>& VertexData)
	{
		return Create(Name, reinterpret_cast<const void*>(&VertexData[0]), sizeof(VertexDataType), VertexData.size());
	}
	static std::shared_ptr<FVertexBuffer> Create(std::string_view Name, const void* InitData,
												 size_t VertexSize, size_t VertexCount);

	ENGINE_API void Update();

protected:
	void AssetCreate(const void* InitData, size_t VertexSize, size_t VertexCount);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
	D3D11_BUFFER_DESC VertexBufferDesc = { 0 };
	UINT VertexSize = 0;
	UINT VertexCount = 0;
};


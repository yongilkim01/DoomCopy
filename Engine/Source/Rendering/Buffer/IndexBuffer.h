#pragma once
#include "Rendering/Buffer/EngineDeviceBuffer.h"
#include "Classes/Engine/RenderAsset.h"

/**
 *	설명
 */
class FIndexBuffer : public URenderAsset, public UEngineDeviceBuffer
{
public:
	/** 생성자, 소멸자 */
	FIndexBuffer();
	~FIndexBuffer();

	/** 객체 값 복사 방지 */
	FIndexBuffer(const FIndexBuffer& Other) = delete;
	FIndexBuffer(FIndexBuffer&& Other) noexcept = delete;
	FIndexBuffer& operator=(const FIndexBuffer& Other) = delete;
	FIndexBuffer& operator=(FIndexBuffer&& Other) noexcept = delete;

	template<typename IndexDataType>
	ENGINE_API static std::shared_ptr<FIndexBuffer> Create(
		std::string_view Name,
		const std::vector<IndexDataType>& IndexData)
	{
		return Create(
			Name, 
			reinterpret_cast<const void*>(&IndexData[0]), 
			sizeof(IndexDataType), 
			IndexData.size());
	}
	ENGINE_API static std::shared_ptr<FIndexBuffer> Create(
		std::string_view Name, 
		const void* InitData,
		size_t IndexSize, 
		size_t IndexCount);

	ENGINE_API void Update();

	UINT GetIndexBufferSize()
	{
		return IndexSize;
	}
	ENGINE_API UINT GetIndexCount()
	{
		return IndexCount;
	}


protected:
	void AssetCreate(const void* InitData, size_t IndexSize, size_t IndexCount);

private:
	UINT IndexSize = 0;
	UINT IndexCount = 0;
	DXGI_FORMAT Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
};


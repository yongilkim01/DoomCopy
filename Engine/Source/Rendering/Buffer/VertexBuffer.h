#pragma once
#include "Rendering/Buffer/EngineDeviceBuffer.h"
#include "Engine/Classes/Engine/EngineResource.h"

class UEngineInputLayoutInfo;

/**
 *	설명
 */
class FVertexBuffer : public UResource, public UEngineDeviceBuffer
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
	ENGINE_API static std::shared_ptr<FVertexBuffer> Create(
		std::string_view Name, 
		const std::vector<VertexDataType>& VertexData)
	{
		return Create(
			Name,
			reinterpret_cast<const void*>(&VertexData[0]),
			sizeof(VertexDataType), 
			VertexData.size(),
			&VertexDataType::GetInfo());
	}
	ENGINE_API static std::shared_ptr<FVertexBuffer> Create(
		std::string_view Name, 
		const void* InitData,
		size_t VertexSize, 
		size_t VertexCount,
		UEngineInputLayoutInfo* _InfoPtr = nullptr);

	ENGINE_API void Update();

	ENGINE_API UEngineInputLayoutInfo* GetInfoPtr()
	{
		return InfoPtr;
	}

protected:
	void AssetCreate(const void* InitData, size_t VertexSize, size_t VertexCount);

private:
	UINT VertexSize = 0;
	UINT VertexCount = 0;

	UEngineInputLayoutInfo* InfoPtr;

};


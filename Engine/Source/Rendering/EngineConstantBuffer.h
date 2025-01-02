#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "Rendering/EngineDeviceBuffer.h"

/**
 *	����
 */
class UEngineConstantBuffer : public URenderAsset, public UEngineDeviceBuffer
{
public:
	/** ������, �Ҹ��� */
	UEngineConstantBuffer();
	~UEngineConstantBuffer();

	/** ��ü �� ���� ���� */
	UEngineConstantBuffer(const UEngineConstantBuffer& Other) = delete;
	UEngineConstantBuffer(UEngineConstantBuffer&& Other) noexcept = delete;
	UEngineConstantBuffer& operator=(const UEngineConstantBuffer& Other) = delete;
	UEngineConstantBuffer& operator=(UEngineConstantBuffer&& Other) noexcept = delete;

	static std::shared_ptr<UEngineConstantBuffer> CreateOrFind(int _Byte, const std::string_view& _Name);

protected:

private:
	void AssetCreate();

	static std::map<int, std::map<std::string, std::shared_ptr<UEngineConstantBuffer>>> BufferMap;
};


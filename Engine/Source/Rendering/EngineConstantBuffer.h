#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "Rendering/EngineDeviceBuffer.h"
#include "EngineEnums.h"

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

	static std::shared_ptr<UEngineConstantBuffer> CreateOrFind(UINT _Byte, const std::string_view& _Name);

	void ChangeData(void* _Data, UINT _Size);
	void Update(EShaderType Type, UINT BindIndex);

protected:

private:
	void AssetCreate(UINT Byte);

	static std::map<int, std::map<std::string, std::shared_ptr<UEngineConstantBuffer>>> BufferMap;
};

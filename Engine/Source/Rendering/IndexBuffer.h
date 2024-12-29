#pragma once
#include "Classes/Engine/RenderAsset.h"

/**
 *	����
 */
class FIndexBuffer : public URenderAsset
{
public:
	/** ������, �Ҹ��� */
	FIndexBuffer();
	~FIndexBuffer();

	/** ��ü �� ���� ���� */
	FIndexBuffer(const FIndexBuffer& Other) = delete;
	FIndexBuffer(FIndexBuffer&& Other) noexcept = delete;
	FIndexBuffer& operator=(const FIndexBuffer& Other) = delete;
	FIndexBuffer& operator=(FIndexBuffer&& Other) noexcept = delete;

	template<typename IndexDataType>
	static std::shared_ptr<FIndexBuffer> Create(std::string_view Name,
											    const std::vector<IndexDataType>& IndexData)
	{
		return Create(Name, reinterpret_cast<const void*>(&IndexData[0]), sizeof(IndexDataType), IndexData.size());
	}
	static std::shared_ptr<FIndexBuffer> Create(std::string_view Name, const void* InitData,
												size_t IndexSize, size_t IndexCount);

protected:
	void AssetCreate(const void* InitData, size_t IndexSize, size_t IndexCount);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer = nullptr;
	D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
	int IndexSize = 0;
	int IndexCount = 0;
};

#pragma once
#include "Engine/Classes/Engine/EngineResource.h"

/**
 *	����
 */
class UEngineBlend : public UResource
{
public:
	/** ������, �Ҹ��� */
	UEngineBlend();
	~UEngineBlend();

	/** ��ü �� ���� ���� */
	UEngineBlend(const UEngineBlend& Other) = delete;
	UEngineBlend(UEngineBlend&& Other) noexcept = delete;
	UEngineBlend& operator=(const UEngineBlend& Other) = delete;
	UEngineBlend& operator=(UEngineBlend&& Other) noexcept = delete;

	static std::shared_ptr<UEngineBlend> Create(std::string_view Name, const D3D11_BLEND_DESC& BlendDesc);

	void Update();

protected:
	void AssetCreate(const D3D11_BLEND_DESC& BlendDesc);

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> State = nullptr;
	FVector BlendFactor = FVector(0.0f, 0.0f, 0.0f, 0.0f);
	UINT Mask = 0xFFFFFFFF;

};
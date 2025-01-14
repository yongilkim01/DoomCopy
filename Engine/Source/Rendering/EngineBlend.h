#pragma once
#include "Engine/Classes/Engine/EngineResource.h"

/**
 *	설명
 */
class UEngineBlend : public UResource
{
public:
	/** 생성자, 소멸자 */
	UEngineBlend();
	~UEngineBlend();

	/** 객체 값 복사 방지 */
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
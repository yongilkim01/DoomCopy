#pragma once
#include "Classes/Engine/RenderAsset.h"

/**
 *	����
 */
class UEngineRasterizerState : public URenderAsset
{
public:
	/** ������, �Ҹ��� */
	UEngineRasterizerState();
	~UEngineRasterizerState();

	/** ��ü �� ���� ���� */
	UEngineRasterizerState(const UEngineRasterizerState& Other) = delete;
	UEngineRasterizerState(UEngineRasterizerState&& Other) noexcept = delete;
	UEngineRasterizerState& operator=(const UEngineRasterizerState& Other) = delete;
	UEngineRasterizerState& operator=(UEngineRasterizerState&& Other) noexcept = delete;

	static std::shared_ptr<UEngineRasterizerState> Create(std::string_view _Name, const D3D11_RASTERIZER_DESC& _Value);
	
	void Update();

protected:

private:
	void AssetCreate(const D3D11_RASTERIZER_DESC& _Value);
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> State = nullptr;
};

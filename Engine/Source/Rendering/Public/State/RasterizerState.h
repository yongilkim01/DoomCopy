#pragma once
#include "Engine/Classes/Engine/EngineResource.h"

/**
 *	����
 */
class URasterizerState : public UResource
{
public:
	/** ������, �Ҹ��� */
	URasterizerState();
	~URasterizerState();

	/** ��ü �� ���� ���� */
	URasterizerState(const URasterizerState& Other) = delete;
	URasterizerState(URasterizerState&& Other) noexcept = delete;
	URasterizerState& operator=(const URasterizerState& Other) = delete;
	URasterizerState& operator=(URasterizerState&& Other) noexcept = delete;

	static std::shared_ptr<URasterizerState> Create(std::string_view _Name, const D3D11_RASTERIZER_DESC& _Value);
	
	void Update();

protected:

private:
	void AssetCreate(const D3D11_RASTERIZER_DESC& _Value);
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> State = nullptr;
};


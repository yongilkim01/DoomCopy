#pragma once
#include "Engine/Classes/Engine/EngineResource.h"

/**
 *	설명
 */
class URasterizerState : public UResource
{
public:
	/** 생성자, 소멸자 */
	URasterizerState();
	~URasterizerState();

	/** 객체 값 복사 방지 */
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


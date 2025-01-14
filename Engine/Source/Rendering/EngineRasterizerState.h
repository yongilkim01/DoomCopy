#pragma once
#include "Engine/Classes/Engine/EngineResource.h"

/**
 *	설명
 */
class UEngineRasterizerState : public UResource
{
public:
	/** 생성자, 소멸자 */
	UEngineRasterizerState();
	~UEngineRasterizerState();

	/** 객체 값 복사 방지 */
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


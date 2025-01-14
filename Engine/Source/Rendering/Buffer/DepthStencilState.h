#pragma once
#include "Engine/Classes/Engine/EngineResource.h"

/**
 *	설명
 */
class UDepthStencilState : public UResource
{
public:
	/** 생성자, 소멸자 */
	UDepthStencilState();
	~UDepthStencilState();

	/** 객체 값 복사 방지 */
	UDepthStencilState(const UDepthStencilState& Other) = delete;
	UDepthStencilState(UDepthStencilState&& Other) noexcept = delete;
	UDepthStencilState& operator=(const UDepthStencilState& Other) = delete;
	UDepthStencilState& operator=(UDepthStencilState&& Other) noexcept = delete;

	static std::shared_ptr<UDepthStencilState> Create(std::string_view DepthStencilName, const D3D11_DEPTH_STENCIL_DESC& Desc);

	void Update();

protected:
	void CreateAsset(const D3D11_DEPTH_STENCIL_DESC& Desc);

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> State = nullptr;

};


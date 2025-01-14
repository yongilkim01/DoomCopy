#include "pch.h"
#include "EngineRasterizerState.h"

UEngineRasterizerState::UEngineRasterizerState()
{
}

UEngineRasterizerState::~UEngineRasterizerState()
{
}

std::shared_ptr<UEngineRasterizerState> UEngineRasterizerState::Create(std::string_view _Name, const D3D11_RASTERIZER_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);
	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 레스터라이저스테이트를 로드하려고 시도했습니다 " + UpperName);
		return nullptr;
	}
	std::shared_ptr<UEngineRasterizerState> NewRasterizeStateAsset = std::make_shared<UEngineRasterizerState>();

	AddAsset<UEngineRasterizerState>(NewRasterizeStateAsset, _Name, "");

	NewRasterizeStateAsset->AssetCreate(_Value);

	return NewRasterizeStateAsset;
}

void UEngineRasterizerState::Update()
{
	UGameEngine::GetDevice().GetDeviceContext()->RSSetState(State.Get());

}

void UEngineRasterizerState::AssetCreate(const D3D11_RASTERIZER_DESC& _Value)
{
	if (UGameEngine::GetDevice().GetDevice()->CreateRasterizerState(&_Value, &State))
	{
		MSGASSERT("블랜드 스테이트 생성에 실패했습니다");
		return;
	}
}


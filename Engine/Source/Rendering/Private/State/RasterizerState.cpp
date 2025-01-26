#include "Engine.h"
#include "Rendering/Public/State/RasterizerState.h"

URasterizerState::URasterizerState()
{
}

URasterizerState::~URasterizerState()
{
}

std::shared_ptr<URasterizerState> URasterizerState::Create(std::string_view _Name, const D3D11_RASTERIZER_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);
	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 레스터라이저스테이트를 로드하려고 시도했습니다 " + UpperName);
		return nullptr;
	}
	std::shared_ptr<URasterizerState> NewRasterizeStateAsset = std::make_shared<URasterizerState>();

	AddAsset<URasterizerState>(NewRasterizeStateAsset, _Name, "");

	NewRasterizeStateAsset->AssetCreate(_Value);

	return NewRasterizeStateAsset;
}

void URasterizerState::Update()
{
	UGameEngine::GetDeviceContext()->RSSetState(State.Get());

}

void URasterizerState::AssetCreate(const D3D11_RASTERIZER_DESC& _Value)
{
	if (UGameEngine::GetDevice()->CreateRasterizerState(&_Value, &State))
	{
		MSGASSERT("블랜드 스테이트 생성에 실패했습니다");
		return;
	}
}


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
		MSGASSERT("�̹� �ε��� �����Ͷ�����������Ʈ�� �ε��Ϸ��� �õ��߽��ϴ� " + UpperName);
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
		MSGASSERT("���� ������Ʈ ������ �����߽��ϴ�");
		return;
	}
}


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
		MSGASSERT("�̹� �ε��� �����Ͷ�����������Ʈ�� �ε��Ϸ��� �õ��߽��ϴ� " + UpperName);
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
		MSGASSERT("���� ������Ʈ ������ �����߽��ϴ�");
		return;
	}
}


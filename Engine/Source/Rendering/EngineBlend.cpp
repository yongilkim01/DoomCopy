#include "pch.h"
#include "EngineBlend.h"

UEngineBlend::UEngineBlend()
{
}

UEngineBlend::~UEngineBlend()
{
}

std::shared_ptr<UEngineBlend> UEngineBlend::Create(std::string_view Name, const D3D11_BLEND_DESC& BlendDesc)
{
	std::string UpperName = ToUpperName(Name);
	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε�� ���� �����Դϴ�" + UpperName);
		return nullptr;
	}
	std::shared_ptr<UEngineBlend> NewRes = std::make_shared<UEngineBlend>();
	AddAsset<UEngineBlend>(NewRes, Name, "");
	NewRes->AssetCreate(BlendDesc);
	return NewRes;
}

void UEngineBlend::Update()
{
	UGameEngine::GetDevice().GetDeviceContext()->OMSetBlendState(State.Get(), BlendFactor.Arr1D, Mask);
}

void UEngineBlend::AssetCreate(const D3D11_BLEND_DESC& BlendDesc)
{
	if (UGameEngine::GetDevice().GetDevice()->CreateBlendState(&BlendDesc, &State))
	{
		MSGASSERT("���� ������Ʈ ������ �����߽��ϴ�");
		return;
	}
}

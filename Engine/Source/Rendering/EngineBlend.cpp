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
		MSGASSERT("이미 로드된 블렌드 에셋입니다" + UpperName);
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
		MSGASSERT("블랜드 스테이트 생성에 실패했습니다");
		return;
	}
}

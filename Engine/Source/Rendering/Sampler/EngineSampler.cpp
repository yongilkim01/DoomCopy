#include "pch.h"
#include "EngineSampler.h"

UEngineSampler::UEngineSampler()
{
}

UEngineSampler::~UEngineSampler()
{
}

void UEngineSampler::Update(EShaderType ShaderType, UINT BindIndex)
{
	ID3D11SamplerState* ArrPtr[1] = { State.Get() };

	switch (ShaderType)
	{
	case EShaderType::VS:
		UGameEngine::GetDeviceContext()->VSSetSamplers(BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UGameEngine::GetDeviceContext()->PSSetSamplers(BindIndex, 1, ArrPtr);
		break;
	case EShaderType::HS:
	case EShaderType::DS:
	case EShaderType::GS:
	case EShaderType::CS:
	default:
		MSGASSERT("지원하지 않는 셰이더에 샘플러를 세팅하려고 했습니다");
		break;
	}
}

std::shared_ptr<UEngineSampler> UEngineSampler::Create(std::string_view Name, const D3D11_SAMPLER_DESC& SamplerDesc)
{
	std::string UpperName = ToUpperName(Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 샘플러를 로드하려고 했습니다 " + UpperName);
		return nullptr;
	}
	std::shared_ptr<UEngineSampler> NewRes = std::make_shared<UEngineSampler>();

	AddAsset<UEngineSampler>(NewRes, Name, "");

	NewRes->AssetCreate(SamplerDesc);
	return NewRes;
}

void UEngineSampler::AssetCreate(const D3D11_SAMPLER_DESC& SamplerDesc)
{
	if (UGameEngine::GetDevice()->CreateSamplerState(&SamplerDesc, &State))
	{
		MSGASSERT("샘플러 생성에 실패했습니다");
		return;
	}
}

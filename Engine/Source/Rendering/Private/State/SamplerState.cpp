#include "pch.h"
#include "Rendering/Public/State/SamplerState.h"

USamplerState::USamplerState()
{
}

USamplerState::~USamplerState()
{
}

void USamplerState::Update(EShaderType ShaderType, UINT BindIndex)
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

void USamplerState::Reset(EShaderType ShaderType, UINT BindIndex)
{
	ID3D11SamplerState* ArrPtr[1] = { nullptr };

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
		MSGASSERT("존재하지 않는 타입의 셰이터를 리셋하려고 했습니다");
		break;
	}
}

std::shared_ptr<USamplerState> USamplerState::Create(std::string_view Name, const D3D11_SAMPLER_DESC& SamplerDesc)
{
	std::string UpperName = ToUpperName(Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 샘플러를 로드하려고 했습니다 " + UpperName);
		return nullptr;
	}
	std::shared_ptr<USamplerState> NewRes = std::make_shared<USamplerState>();

	AddAsset<USamplerState>(NewRes, Name, "");

	NewRes->AssetCreate(SamplerDesc);
	return NewRes;
}

void USamplerState::AssetCreate(const D3D11_SAMPLER_DESC& SamplerDesc)
{
	if (UGameEngine::GetDevice()->CreateSamplerState(&SamplerDesc, &State))
	{
		MSGASSERT("샘플러 생성에 실패했습니다");
		return;
	}
}

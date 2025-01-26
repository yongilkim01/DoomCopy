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
		MSGASSERT("�������� �ʴ� ���̴��� ���÷��� �����Ϸ��� �߽��ϴ�");
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
		MSGASSERT("�������� �ʴ� Ÿ���� �����͸� �����Ϸ��� �߽��ϴ�");
		break;
	}
}

std::shared_ptr<USamplerState> USamplerState::Create(std::string_view Name, const D3D11_SAMPLER_DESC& SamplerDesc)
{
	std::string UpperName = ToUpperName(Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� ���÷��� �ε��Ϸ��� �߽��ϴ� " + UpperName);
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
		MSGASSERT("���÷� ������ �����߽��ϴ�");
		return;
	}
}

#include "pch.h"
#include "Engine/Public/Materials/Material.h"

UEngineMaterial::UEngineMaterial()
{
	SetRasterizerState("EngineBase");
	SetBlend("AlphaBlend");
	SetDepthStencilState("BaseDepth");

}

UEngineMaterial::~UEngineMaterial()
{
}

std::shared_ptr<UEngineMaterial> UEngineMaterial::Create(std::string_view _Name)
{
	std::string UpperName = ToUpperName(_Name);
	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �����ϴ� ��Ƽ������ �� ������� �߽��ϴ�." + UpperName);
		return nullptr;
	}
	std::shared_ptr<UEngineMaterial> NewRes = std::make_shared<UEngineMaterial>();
	AddAsset<UEngineMaterial>(NewRes, _Name, "");
	return NewRes;
}

void UEngineMaterial::UpdatePrimitiveTopology()
{
	UGameEngine::GetDevice().GetDeviceContext()->IASetPrimitiveTopology(TOPOLOGY);
}

void UEngineMaterial::SetVertexShader(std::string_view _Name)
{
	VertexShader = UEngineVertexShader::Find<UEngineVertexShader>(_Name);
	if (nullptr == VertexShader)
	{
		MSGASSERT("�������� �ʴ� ���ؽ� ���̴��� �����Ϸ��� �߽��ϴ�.");
	}
}
void UEngineMaterial::SetPixelShader(std::string_view _Name)
{
	PixelShader = UEnginePixelShader::Find<UEnginePixelShader>(_Name);
	if (nullptr == PixelShader)
	{
		MSGASSERT("�������� �ʴ� �ȼ� ���̴��� �����Ϸ��� �߽��ϴ�.");
	}
}
void UEngineMaterial::SetRasterizerState(std::string_view _Name)
{
	RasterizerState = UEngineRasterizerState::Find<UEngineRasterizerState>(_Name);
	if (nullptr == RasterizerState)
	{
		MSGASSERT("�������� �ʴ� ���ؽ� ���̴��� �����Ϸ��� �߽��ϴ�.");
	}
}
void UEngineMaterial::SetBlend(std::string_view _Name)
{
	Blend = UEngineBlend::Find<UEngineBlend>(_Name);
	if (nullptr == Blend)
	{
		MSGASSERT("�������� �ʴ� ���ؽ� ���̴��� �����Ϸ��� �߽��ϴ�.");
	}
}

void UEngineMaterial::SetDepthStencilState(std::string_view NewDepthName)
{
	DepthState = UDepthStencilState::Find<UDepthStencilState>(NewDepthName);

	if (nullptr == DepthState)
	{
		MSGASSERT("�������� �ʴ� ���̹��۸� �����Ϸ��� �߽��ϴ�");
	}
}

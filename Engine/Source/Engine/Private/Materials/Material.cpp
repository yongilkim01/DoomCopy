#include "Engine.h"
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
		MSGASSERT("이미 존재하는 머티리얼을 또 만들려고 했습니다." + UpperName);
		return nullptr;
	}
	std::shared_ptr<UEngineMaterial> NewRes = std::make_shared<UEngineMaterial>();
	AddAsset<UEngineMaterial>(NewRes, _Name, "");
	return NewRes;
}

void UEngineMaterial::UpdatePrimitiveTopology()
{
	UGameEngine::GetDeviceContext()->IASetPrimitiveTopology(TOPOLOGY);
}

void UEngineMaterial::SetVertexShader(std::string_view _Name)
{
	VertexShader = UEngineVertexShader::Find<UEngineVertexShader>(_Name);
	if (nullptr == VertexShader)
	{
		MSGASSERT("존재하지 않는 버텍스 쉐이더를 생성하려고 했습니다.");
	}
}
void UEngineMaterial::SetPixelShader(std::string_view _Name)
{
	PixelShader = UEnginePixelShader::Find<UEnginePixelShader>(_Name);
	if (nullptr == PixelShader)
	{
		MSGASSERT("존재하지 않는 픽셀 셰이더를 생성하려고 했습니다.");
	}
}
void UEngineMaterial::SetRasterizerState(std::string_view _Name)
{
	RasterizerState = URasterizerState::Find<URasterizerState>(_Name);
	if (nullptr == RasterizerState)
	{
		MSGASSERT("존재하지 않는 버텍스 쉐이더를 생성하려고 했습니다.");
	}
}
void UEngineMaterial::SetBlend(std::string_view _Name)
{
	Blend = UEngineBlend::Find<UEngineBlend>(_Name);
	if (nullptr == Blend)
	{
		MSGASSERT("존재하지 않는 버텍스 쉐이더를 생성하려고 했습니다.");
	}
}

void UEngineMaterial::SetDepthStencilState(std::string_view NewDepthName)
{
	DepthState = UDepthStencilState::Find<UDepthStencilState>(NewDepthName);

	if (nullptr == DepthState)
	{
		MSGASSERT("존재하지 않는 깊이버퍼를 생성하려고 했습니다");
	}
}

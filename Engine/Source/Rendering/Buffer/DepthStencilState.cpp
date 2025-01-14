#include "pch.h"
#include "DepthStencilState.h"

#include "Engine/Classes/Engine/GameEngine.h"

UDepthStencilState::UDepthStencilState()
{
}

UDepthStencilState::~UDepthStencilState()
{
}

std::shared_ptr<UDepthStencilState> UDepthStencilState::Create(std::string_view DepthStencilName, const D3D11_DEPTH_STENCIL_DESC& Desc)
{
	std::string UpperName = ToUpperName(DepthStencilName);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드된 깊이 버퍼를 로드하려고 시도했습니다 " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UDepthStencilState> NewResource = std::make_shared<UDepthStencilState>();
	AddAsset<UDepthStencilState>(NewResource, DepthStencilName, "");
	NewResource->CreateAsset(Desc);

	return NewResource;
}

void UDepthStencilState::Update()
{
	UEngineCore::GetDevice().GetDeviceContext()->OMSetDepthStencilState(State.Get(), 0);
}

void UDepthStencilState::CreateAsset(const D3D11_DEPTH_STENCIL_DESC& Desc)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateDepthStencilState(&Desc, &State))
	{
		MSGASSERT("깊이 버퍼 생성에 실패했습니다");
		return;
	}
}

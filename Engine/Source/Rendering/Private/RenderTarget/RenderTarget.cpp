#include "Engine.h"
#include "Rendering/Public/RenderTarget/RenderTarget.h"

#include "Engine/Classes/Engine/Texture.h"

URenderTarget::URenderTarget()
{
	RenderTargetUnit.SetMesh("FullRect");
	RenderTargetUnit.SetMaterial("RenderTargetMaterial");
}

URenderTarget::~URenderTarget()
{
}

void URenderTarget::CreateTarget(Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D)
{
	std::shared_ptr<UTexture> NewRenderTargetTexture = std::make_shared<UTexture>();
	NewRenderTargetTexture->CreateAsset(Texture2D);

	if (nullptr == NewRenderTargetTexture->GetRenderTargetView())
	{
		MSGASSERT("렌더 타겟 뷰가 존재하지 않습니다");
		return;
	}

	RenderTargetViewVector.push_back(NewRenderTargetTexture->GetRenderTargetView());
	RenderTargetTextureVector.push_back(NewRenderTargetTexture);
}

void URenderTarget::CreateTarget(float4 InScale, float4 InClearColor, DXGI_FORMAT InFormat)
{
	ClearColor = InClearColor;

	std::shared_ptr<UTexture> NewRenderTargetTexture = std::make_shared<UTexture>();

	D3D11_TEXTURE2D_DESC Desc = { 0 };
	Desc.ArraySize = 1;
	Desc.Width = InScale.iX();
	Desc.Height = InScale.iY();
	Desc.Format = InFormat;

	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	Desc.MipLevels = 1;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.CPUAccessFlags = 0;
	Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

	NewRenderTargetTexture->CreateAsset(Desc);
	NewRenderTargetTexture->SetTextureSize(InScale);

	RenderTargetViewVector.push_back(NewRenderTargetTexture->GetRenderTargetView());
	RenderTargetTextureVector.push_back(NewRenderTargetTexture);
}

void URenderTarget::CreateDepthStencil(int Index)
{
	if (RenderTargetTextureVector.size() <= Index)
	{
		MSGASSERT("인덱스의 범위가 텍스처 배열의 범위를 벗어난 값입니다");
	}

	FVector Size = RenderTargetTextureVector[Index]->GetTextureSize();

	D3D11_TEXTURE2D_DESC Desc = { 0 };
	
	Desc.ArraySize = 1;
	Desc.Width = Size.iX();
	Desc.Height = Size.iY();
	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.MipLevels = 1;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.CPUAccessFlags = 0;
	Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	
	DepthStencilTexture = std::make_shared<UTexture>();
	DepthStencilTexture->CreateAsset(Desc);
}

void URenderTarget::Clear()
{
	for (size_t i = 0; i < RenderTargetViewVector.size(); i++)
	{
		UGameEngine::GetDeviceContext()->ClearRenderTargetView(
			RenderTargetViewVector[i], ClearColor.Arr1D);
	}
	UGameEngine::GetDeviceContext()->ClearDepthStencilView(
		DepthStencilTexture->GetDepthStencilView(), 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);
}

void URenderTarget::Setting()
{
	UGameEngine::GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetViewVector[0], DepthStencilTexture->GetDepthStencilView());
}

void URenderTarget::CopyRenderTarget(std::shared_ptr<URenderTarget> CopyToRenderTarget)
{
	CopyToRenderTarget->Clear();
	MergeRenderTarget(CopyToRenderTarget);
}

void URenderTarget::MergeRenderTarget(std::shared_ptr<URenderTarget> MergeToRenderTarget)
{
	MergeToRenderTarget->Setting();

	RenderTargetUnit.SetTexture("MergeTex", RenderTargetTextureVector[0]);
	RenderTargetUnit.Render(nullptr, 0.0f);
	RenderTargetUnit.Reset();
}

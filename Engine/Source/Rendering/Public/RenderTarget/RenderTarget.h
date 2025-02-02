#pragma once
#include "Engine/Classes/Engine/EngineResource.h"
#include "Engine/Classes/Engine/Texture.h"
#include "Rendering/Public/Device/GraphicDevice.h"
#include "Engine/Classes/Engine/RenderAsset.h"

/**
 *	����
 */
class URenderTarget : public UResource
{
public:
	/** ������, �Ҹ��� */
	URenderTarget();
	~URenderTarget();

	/** ��ü �� ���� ���� */
	URenderTarget(const URenderTarget& Other) = delete;
	URenderTarget(URenderTarget&& Other) noexcept = delete;
	URenderTarget& operator=(const URenderTarget& Other) = delete;
	URenderTarget& operator=(URenderTarget&& Other) noexcept = delete;

	ENGINE_API void CreateTarget(Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D);
	ENGINE_API void CreateTarget(float4 InScale, float4 InClearColor = float4::NONE, DXGI_FORMAT InFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	ENGINE_API void CreateDepthStencil(int Index = 0);
	ENGINE_API void Clear();
	ENGINE_API void Setting();
	ENGINE_API void CopyRenderTarget(std::shared_ptr<URenderTarget> CopyToRenderTarget);
	ENGINE_API void MergeRenderTarget(std::shared_ptr<URenderTarget> MergeToRenderTarget);

	/** ��, �� �޼ҵ� */
	ENGINE_API void SetClearColor(float4 Value)
	{
		ClearColor = Value;
	}

protected:

private:
	float4 ClearColor = float4(0.20f, 0.20f, 0.20f, 1.0f);

	std::vector<std::shared_ptr<UTexture>> RenderTargetTextureVector;
	std::vector<ID3D11RenderTargetView*> RenderTargetViewVector;
	std::vector<ID3D11ShaderResourceView*> ShaderResourceViewVector;

	std::shared_ptr<UTexture> DepthStencilTexture;

	URenderAsset RenderTargetUnit;
};


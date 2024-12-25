#pragma once
#include "Classes/Engine/AssetManager.h"
#include "ThirdParty/DirectXTex/Include/DirectXTex.h"

/**
 *	설명
 */
class UTexture : public UAssetManager
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UTexture();
	ENGINE_API ~UTexture();

	/** 객체 값 복사 방지 */
	UTexture(const UTexture& _Other) = delete;
	UTexture(UTexture&& _Other) noexcept = delete;
	UTexture& operator=(const UTexture& _Other) = delete;
	UTexture& operator=(UTexture&& _Other) noexcept = delete;

	static std::shared_ptr<UTexture> Load(std::string_view LoadTextureFilePath)
	{
		FPaths TextureFilePath = FPaths(LoadTextureFilePath);

		std::string FileName = TextureFilePath.GetFileName();

		return Load(FileName, LoadTextureFilePath);
	}

	static std::shared_ptr<UTexture> Load(std::string_view TextureFileName, std::string_view LoadTextureFilePath);

	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return ShaderResourceView.Get();
	}

	FVector GetTextureSize()
	{
		return TextureSize;
	}

protected:

private:
	ENGINE_API void AssetLoad();

	FVector TextureSize;
	DirectX::TexMetadata Metadata;
	DirectX::ScratchImage ImageData;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D = nullptr; // 로드한 텍스처
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView = nullptr; // 텍스처를 쉐이더 세팅할수 있는권한
};


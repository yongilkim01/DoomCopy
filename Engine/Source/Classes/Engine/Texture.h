#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "ThirdParty/DirectXTex/Include/DirectXTex.h"
#include "Core/Misc/Paths.h"
#include "EngineEnums.h"


/**
 *	설명
 */
class UTexture : public URenderAsset
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

	void Update(EShaderType ShaderType, UINT BindIndex);

	ENGINE_API void CreateAsset(const D3D11_TEXTURE2D_DESC& InitTextureDesc);

	//static std::shared_ptr<UTexture> LoadToObjFile(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene, std::string_view LoadTextureFilePath);

	static std::shared_ptr<UTexture> Load(std::string_view LoadTextureFilePath)
	{
		FPaths TextureFilePath = FPaths(LoadTextureFilePath);

		std::string FileName = TextureFilePath.GetFileName();

		return Load(FileName, LoadTextureFilePath);
	}

	ENGINE_API static std::shared_ptr<UTexture> Load(std::string_view TextureFileName, std::string_view LoadTextureFilePath);

	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return ShaderResourceView.Get();
	}

	FVector GetTextureSize()
	{
		return TextureSize;
	}

	/** 겟, 셋 메소드 */
	std::string GetTextureType()
	{
		return TextureType;
	}
	void SetTextureType(std::string_view NewType)
	{
		TextureType = NewType;
	}
	std::string GetTexturePath()
	{
		return TexturePath;
	}
	bool SetTxturePath(std::string_view NewPath)
	{
		TexturePath = NewPath;

		return true;
	}
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return RenderTargetView.Get();
	}
	ID3D11DepthStencilView* GetDepthStencilView()
	{
		return DepthStencilView.Get();
	}

protected:

private:
	ENGINE_API void AssetLoad();

	FVector TextureSize;
	DirectX::TexMetadata Metadata;
	DirectX::ScratchImage ImageData;

	std::string TextureType = "";
	std::string TexturePath = "";

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D = nullptr; // 로드한 텍스처
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView = nullptr; // 텍스처를 쉐이더 세팅할수 있는권한
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView = nullptr; // 텍스처를 쉐이더 세팅할수 있는권한
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr; // 텍스처를 쉐이더 세팅할수 있는권한
	D3D11_TEXTURE2D_DESC TextureDesc;
};


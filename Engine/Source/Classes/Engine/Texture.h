#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "ThirdParty/DirectXTex/Include/DirectXTex.h"
#include "Core/Misc/Paths.h"
#include "EngineEnums.h"

/**
 *	����
 */
class UTexture : public URenderAsset
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UTexture();
	ENGINE_API ~UTexture();

	/** ��ü �� ���� ���� */
	UTexture(const UTexture& _Other) = delete;
	UTexture(UTexture&& _Other) noexcept = delete;
	UTexture& operator=(const UTexture& _Other) = delete;
	UTexture& operator=(UTexture&& _Other) noexcept = delete;

	void Update(EShaderType ShaderType, UINT BindIndex);

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

	/** ��, �� �޼ҵ� */
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

protected:

private:
	ENGINE_API void AssetLoad();

	FVector TextureSize;
	DirectX::TexMetadata Metadata;
	DirectX::ScratchImage ImageData;

	std::string TextureType = "";
	std::string TexturePath = "";

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D = nullptr; // �ε��� �ؽ�ó
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView = nullptr; // �ؽ�ó�� ���̴� �����Ҽ� �ִ±���
};


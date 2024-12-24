#pragma once
#include "Classes/Engine/AssetManager.h"
#include "ThirdParty/DirectXTex/Include/DirectXTex.h"

/**
 *	����
 */
class UTexture : public UAssetManager
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

	static std::shared_ptr<UTexture> Load(std::string_view TextureFilePath)
	{
		FPaths TexturePath = FPaths(TextureFilePath);

		std::string FileName = TexturePath.GetFileName();

		return Load(FileName, TextureFilePath);
	}

	static std::shared_ptr<UTexture> Load(std::string_view TextureFileName, std::string_view TextureFilePath);

	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return ShaderResourceView.Get();
	}

protected:

private:
	ENGINE_API void AssetLoad();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D = nullptr; // �ε��� �ؽ�ó
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView = nullptr; // �ؽ�ó�� ���̴� �����Ҽ� �ִ±���
};


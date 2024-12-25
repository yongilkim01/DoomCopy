#include "pch.h"
#include "Texture.h"

#include "Core/Misc/FileHelper.h"

#ifdef _DEBUG
	#pragma comment(lib, "DirectXTex_Debug.lib")
#else
	#pragma comment(lib, "DirectXTex_Release.lib")
#endif

UTexture::UTexture()
{
}

UTexture::~UTexture()
{
}

std::shared_ptr<UTexture> UTexture::Load(std::string_view TextureFileName, std::string_view LoadTextureFilePath)
{
	std::string UpperName = ToUpperName(TextureFileName);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� �ؽ�ó�� �ε�õ� " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UTexture> NewTexture = std::make_shared<UTexture>();
	AddAsset<UTexture>(NewTexture, TextureFileName, LoadTextureFilePath);
	NewTexture->AssetLoad();

	return NewTexture;
}

void UTexture::AssetLoad()
{
	FFileHelper FileHelper = Path;

	// ���� ��θ� ���ڿ��� ��ȯ
	std::string Str = FileHelper.GetPathToString();
	// ���� Ȯ���ڸ� ������
	std::string Ext = FileHelper.GetExtension();

	// ���ڿ� ��θ� ���̵� ���ڿ��� ��ȯ
	std::wstring WLoadPath = UEngineString::AnsiToUnicode(Str.c_str());
	// Ȯ���ڸ� �빮�ڷ� ��ȯ
	std::string UpperExt = UEngineString::ToUpper(Ext.c_str());

	// ���� Ȯ���ڿ� ���� ������ �Լ��� ���� �ε�
	if (UpperExt == ".DDS")
	{
		// DDS ���� �ε�
		if (S_OK != DirectX::LoadFromDDSFile(WLoadPath.c_str(), DirectX::DDS_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("DDS ���� �ε忡 ����");
			return;
		}
	}
	else if (UpperExt == ".TGA")
	{
		// TGA ���� �ε�
		if (S_OK != DirectX::LoadFromTGAFile(WLoadPath.c_str(), DirectX::TGA_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("TGA ���� �ε忡 ����");
			return;
		}
	}
	else
	{
		// �� ���� ���� ����(WIC ���)
		if (S_OK != DirectX::LoadFromWICFile(WLoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT(UpperExt + " ���� �ε忡 ����");
			return;
		}
	}

	// ���̴� ���ҽ� �� ����
	if (S_OK != DirectX::CreateShaderResourceView(
		UEngineCore::Device.GetDevice(),	// ����̽� ��ü
		ImageData.GetImages(),				// �̹��� ������ �迭
		ImageData.GetImageCount(),			// �̹��� ����
		ImageData.GetMetadata(),			// �̹��� ��Ÿ������
		ShaderResourceView.GetAddressOf()   // ������ ���̴� ���ҽ� ��
	))
	{
		MSGASSERT(UpperExt + "���̴� ���ҽ� �� ������ ����");
		return;
	}

	TextureSize.X = static_cast<float>(Metadata.width);
	TextureSize.Y = static_cast<float>(Metadata.height);
}


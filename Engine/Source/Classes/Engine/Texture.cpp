#include "pch.h"
#include "Texture.h"

#include "Core/Misc/FileHelper.h"

UTexture::UTexture()
{
}

UTexture::~UTexture()
{
}

std::shared_ptr<UTexture> UTexture::Load(std::string_view TextureFileName, std::string_view TextureFilePath)
{
	std::string UpperName = ToUpperName(TextureFileName);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� �ؽ�ó�� �ε�õ� " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UTexture> NewTexture = std::make_shared<UTexture>();
	AddAsset<UTexture>(NewTexture, TextureFileName, TextureFilePath);
	NewTexture->AssetLoad();

	return NewTexture;
}

void UTexture::AssetLoad()
{
	FFileHelper File = Path;

	std::string FilePathStr = File.GetPathToString();
	std::string FileExtension = File.GetExtension();
	std::wstring wLoadPath = UEngineString::AnsiToUnicode(FilePathStr.c_str());
	std::string UpperExtension = UEngineString::ToUpper(FileExtension.c_str());

	DirectX::TexMetadata Metadata;
	DirectX::ScratchImage Image;

	if (UpperExtension == ".DDS")
	{
		if (S_OK != DirectX::LoadFromDDSFile(wLoadPath.c_str(), DirectX::DDS_FLAGS_NONE, &Metadata, Image))
		{
			MSGASSERT("DDS ���� �ε忡 ����");
			return;
		}
	}
	else if (UpperExtension == ".TGA")
	{
		if (S_OK != DirectX::LoadFromTGAFile(wLoadPath.c_str(), DirectX::TGA_FLAGS_NONE, &Metadata, Image))
		{
			MSGASSERT("TGA ���� �ε忡 ����");
			return;
		}
	}
	else
	{
		if (S_OK != DirectX::LoadFromWICFile(wLoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, Image))
		{
			MSGASSERT(UpperExtension + "���� �ε忡 ����");
			return;
		}
	}

	if (S_OK != DirectX::CreateShaderResourceView(
		UEngineCore::Device.GetDevice(),
		Image.GetImages(),
		Image.GetImageCount(),
		Image.GetMetadata(),
		ShaderResourceView.GetAddressOf()
	))
	{
		MSGASSERT(UpperExtension + " ���̴� ���ҽ� �� ������ ����");
		return;
	}
}


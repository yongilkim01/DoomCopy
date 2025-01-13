#include "pch.h"
#include "Engine/Classes/Engine/Texture.h"

#include "Rendering/Public/RenderTarget/RenderTarget.h"

#include "Core/Misc/Paths.h"
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

void UTexture::Update(EShaderType ShaderType, UINT BindIndex)
{
	//std::string PathString = Path.GetPathToString();
	ID3D11ShaderResourceView* ArrPtr[1] = { ShaderResourceView.Get() };

	switch (ShaderType)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetDeviceContext()->VSSetShaderResources(BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetDeviceContext()->PSSetShaderResources(BindIndex, 1, ArrPtr);
		break;
	case EShaderType::HS:
	case EShaderType::DS:
	case EShaderType::GS:
	case EShaderType::CS:
	default:
		MSGASSERT("������ �������� �ʴ� ���̴��� �����Ϸ��� �߽��ϴ�.");
		break;
	}
}

void UTexture::CreateAsset(const D3D11_TEXTURE2D_DESC& InitTextureDesc)
{
	TextureDesc = InitTextureDesc;

	UEngineCore::GetDevice().GetDevice()->CreateTexture2D(&TextureDesc, nullptr, &Texture2D);

	if (nullptr == Texture2D)
	{
		MSGASSERT("�ؽ�ó ������ �����߽��ϴ�");
		return;
	}

	if (TextureDesc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		CreateShaderResourceView();
	}

	if (TextureDesc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		CreateRenderTargetView();
	}

	if (TextureDesc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		CreateDepthStencilView();
	}
}

void UTexture::CreateAsset(Microsoft::WRL::ComPtr<ID3D11Texture2D> InTexture2D)
{
	Texture2D = InTexture2D;
	Texture2D->GetDesc(&TextureDesc);

	TextureSize.X = static_cast<float>(TextureDesc.Width);
	TextureSize.Y = static_cast<float>(TextureDesc.Height);

	CreateRenderTargetView();
}

void UTexture::CreateRenderTargetView()
{
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateRenderTargetView(Texture2D.Get(), nullptr, &RenderTargetView))
	{
		MSGASSERT("�ؽ�ó �������� ȹ�濡 �����߽��ϴ�");
		return;
	}
}

void UTexture::CreateShaderResourceView()
{
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateShaderResourceView(Texture2D.Get(), nullptr, &ShaderResourceView))
	{
		MSGASSERT("���̴� ���ҽ� �� ������ �����߽��ϴ�");
		return;
	}
}

void UTexture::CreateDepthStencilView()
{
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateDepthStencilView(Texture2D.Get(), nullptr, &DepthStencilView))
	{
		MSGASSERT("���� ���� ������ �����߽��ϴ�");
	}
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
	NewTexture->SetTxturePath(LoadTextureFilePath);

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
		UEngineCore::GetDevice().GetDevice(),	// ����̽� ��ü
		ImageData.GetImages(),				// �̹��� ������ �迭
		ImageData.GetImageCount(),			// �̹��� ����
		ImageData.GetMetadata(),			// �̹��� ��Ÿ������
		&ShaderResourceView   // ������ ���̴� ���ҽ� ��
	))
	{
		MSGASSERT(UpperExt + "���̴� ���ҽ� �� ������ ����");
		return;
	}

	TextureSize.X = static_cast<float>(Metadata.width);
	TextureSize.Y = static_cast<float>(Metadata.height);
}


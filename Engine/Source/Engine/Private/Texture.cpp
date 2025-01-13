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
		MSGASSERT("엔진이 지원하지 않는 셰이더에 세팅하려고 했습니다.");
		break;
	}
}

void UTexture::CreateAsset(const D3D11_TEXTURE2D_DESC& InitTextureDesc)
{
	TextureDesc = InitTextureDesc;

	UEngineCore::GetDevice().GetDevice()->CreateTexture2D(&TextureDesc, nullptr, &Texture2D);

	if (nullptr == Texture2D)
	{
		MSGASSERT("텍스처 생성에 실패했습니다");
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
		MSGASSERT("텍스처 수정권한 획득에 실패했습니다");
		return;
	}
}

void UTexture::CreateShaderResourceView()
{
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateShaderResourceView(Texture2D.Get(), nullptr, &ShaderResourceView))
	{
		MSGASSERT("셰이더 리소스 뷰 생성에 실패했습니다");
		return;
	}
}

void UTexture::CreateDepthStencilView()
{
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateDepthStencilView(Texture2D.Get(), nullptr, &DepthStencilView))
	{
		MSGASSERT("깊이 버퍼 생성에 실패했습니다");
	}
}

std::shared_ptr<UTexture> UTexture::Load(std::string_view TextureFileName, std::string_view LoadTextureFilePath)
{
	std::string UpperName = ToUpperName(TextureFileName);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 텍스처를 로드시도 " + UpperName);
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

	// 파일 경로를 문자열로 변환
	std::string Str = FileHelper.GetPathToString();
	// 파일 확장자를 가져옴
	std::string Ext = FileHelper.GetExtension();

	// 문자열 경로를 와이드 문자열로 변환
	std::wstring WLoadPath = UEngineString::AnsiToUnicode(Str.c_str());
	// 확장자를 대문자로 변환
	std::string UpperExt = UEngineString::ToUpper(Ext.c_str());

	// 파일 확장자에 따라 적절한 함수로 파일 로드
	if (UpperExt == ".DDS")
	{
		// DDS 파일 로드
		if (S_OK != DirectX::LoadFromDDSFile(WLoadPath.c_str(), DirectX::DDS_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("DDS 파일 로드에 실패");
			return;
		}
	}
	else if (UpperExt == ".TGA")
	{
		// TGA 파일 로드
		if (S_OK != DirectX::LoadFromTGAFile(WLoadPath.c_str(), DirectX::TGA_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("TGA 파일 로드에 실패");
			return;
		}
	}
	else
	{
		// 그 외의 파일 형식(WIC 사용)
		if (S_OK != DirectX::LoadFromWICFile(WLoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT(UpperExt + " 파일 로드에 실패");
			return;
		}
	}

	// 쉐이더 리소스 뷰 생성
	if (S_OK != DirectX::CreateShaderResourceView(
		UEngineCore::GetDevice().GetDevice(),	// 디바이스 객체
		ImageData.GetImages(),				// 이미지 데이터 배열
		ImageData.GetImageCount(),			// 이미지 개수
		ImageData.GetMetadata(),			// 이미지 메타데이터
		&ShaderResourceView   // 생성된 쉐이더 리소스 뷰
	))
	{
		MSGASSERT(UpperExt + "쉐이더 리소스 뷰 생성에 실패");
		return;
	}

	TextureSize.X = static_cast<float>(Metadata.width);
	TextureSize.Y = static_cast<float>(Metadata.height);
}


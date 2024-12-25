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
		MSGASSERT("이미 로드한 텍스처를 로드시도 " + UpperName);
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
		UEngineCore::Device.GetDevice(),	// 디바이스 객체
		ImageData.GetImages(),				// 이미지 데이터 배열
		ImageData.GetImageCount(),			// 이미지 개수
		ImageData.GetMetadata(),			// 이미지 메타데이터
		ShaderResourceView.GetAddressOf()   // 생성된 쉐이더 리소스 뷰
	))
	{
		MSGASSERT(UpperExt + "쉐이더 리소스 뷰 생성에 실패");
		return;
	}

	TextureSize.X = static_cast<float>(Metadata.width);
	TextureSize.Y = static_cast<float>(Metadata.height);
}


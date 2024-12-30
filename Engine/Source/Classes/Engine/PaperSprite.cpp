#include "pch.h"
#include "PaperSprite.h"

#include "Classes/Engine/Texture.h"
#include "WorldPartition/DebugHelpers.h"
#include "Core/Misc/Paths.h"
#include "Core/Misc/FileHelper.h"


UPaperSprite::UPaperSprite()
{
}

UPaperSprite::~UPaperSprite()
{
}

std::shared_ptr<UPaperSprite> UPaperSprite::CreateSpriteToFolder(std::string_view DirectoryName, std::string_view TexturePath)
{
	FDirectoryHelper DirectoryHelper = TexturePath;

	std::vector<FFileHelper> Files = DirectoryHelper.GetAllFile(false, { ".png" });

	if (0 == Files.size())
	{
		MSGASSERT("파일이 존재하지 않는 폴더를 스프라이트로 만들수는 없습니다.");
	}

	std::shared_ptr<UPaperSprite> NewRes = std::make_shared<UPaperSprite>();
	AddAsset<UPaperSprite>(NewRes, DirectoryName, "");

	for (size_t i = 0; i < Files.size(); i++)
	{
		std::string UpperTextureName = UEngineString::ToUpper(Files[i].GetFileName());

		std::shared_ptr<UTexture> Texture = UTexture::Find<UTexture>(UpperTextureName);

		if (nullptr == Texture)
		{
			MSGASSERT("텍스처를 먼저 로드하고 폴더 스프라이트를 만들어 주세요." + UpperTextureName);
			return nullptr;
		}

		NewRes->TextureVector.push_back(Texture.get());

		FPaperSpriteData SpriteData;

		SpriteData.CuttingLocation = { 0.0f, 0.0f };
		SpriteData.CuttingSize = { 1.0f, 1.0f };
		SpriteData.Pivot = { 0.5f, 0.0f };

		NewRes->SpriteDataVector.push_back(SpriteData);
	}

	return NewRes;
}


std::shared_ptr<UPaperSprite> UPaperSprite::CreateSpriteToMeta(std::string_view _Name, std::string_view _DataFileExt)
{
	std::shared_ptr<UTexture> Tex = UTexture::Find<UTexture>(_Name);

	if (nullptr == Tex)
	{
		MSGASSERT("존재하지 않는 텍스처로 스프라이트를 만들수는 없습니다.");
		return nullptr;
	}

	std::shared_ptr<UPaperSprite> NewRes = std::make_shared<UPaperSprite>();
	AddAsset<UPaperSprite>(NewRes, _Name, "");

	FPaths Path = Tex->GetPath();
	std::string FileName = Path.GetFileName();
	FileName += _DataFileExt;
	Path.MoveParent();
	Path.Append(FileName);

	FFileHelper File = Path;

	File.FileOpen("rt");

	std::string Text = File.GetAllFileText();

	std::vector<std::string> SpriteDataTexts;

	size_t StartPosition = 0;
	while (true)
	{
		size_t RectIndex = Text.find("rect:", StartPosition);
		size_t AligIndex = Text.find("outline:", RectIndex);

		if (RectIndex == std::string::npos || AligIndex == std::string::npos)
		{
			break;
		}

		NewRes->TextureVector.push_back(Tex.get());
		SpriteDataTexts.push_back(Text.substr(RectIndex, AligIndex - RectIndex));
		StartPosition = AligIndex;
	}

	FVector TexSize = Tex->GetTextureSize();

	std::vector<FPaperSpriteData> TestData;

	for (size_t i = 0; i < SpriteDataTexts.size(); i++)
	{
		std::string Text = SpriteDataTexts[i];

		size_t Start = 0;

		FPaperSpriteData SpriteData;


		{
			std::string Number = UEngineString::InterString(Text, "x:", "\n", Start);
			SpriteData.CuttingLocation.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "y:", "\n", Start);
			SpriteData.CuttingLocation.Y = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "width:", "\n", Start);
			SpriteData.CuttingSize.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "height:", "\n", Start);
			SpriteData.CuttingSize.Y = static_cast<float>(atof(Number.c_str()));
		}
		{
			std::string Number = UEngineString::InterString(Text, "x:", ",", Start);
			SpriteData.Pivot.X = static_cast<float>(atof(Number.c_str()));
		}
		{
			std::string Number = UEngineString::InterString(Text, "y:", "}", Start);
			SpriteData.Pivot.Y = static_cast<float>(atof(Number.c_str()));
		}


		SpriteData.CuttingLocation.Y = TexSize.Y - SpriteData.CuttingLocation.Y - SpriteData.CuttingSize.Y;

		SpriteData.CuttingLocation.X /= TexSize.X;
		SpriteData.CuttingLocation.Y /= TexSize.Y;
		SpriteData.CuttingSize.X /= TexSize.X;
		SpriteData.CuttingSize.Y /= TexSize.Y;


		TestData.push_back(SpriteData);
	}


	NewRes->SpriteDataVector = TestData;

	return NewRes;

}

ID3D11ShaderResourceView* UPaperSprite::GetShaderResourceView(size_t Index /* = 0*/)
{
	return TextureVector[0]->GetShaderResourceView();
}

FPaperSpriteData UPaperSprite::GetSpriteData(size_t Index)
{
	if (SpriteDataVector.size() <= Index)
	{
		MSGASSERT("스프라이트의 인덱스를 초과하여 사용하려고 했습니다.");
	}

	return SpriteDataVector[Index];
}

FVector UPaperSprite::GetSpriteScaleToReal(size_t Index)
{
	if (SpriteDataVector.size() <= Index)
	{
		MSGASSERT("스프라이트의 인덱스를 초과하여 사용하려고 했습니다.");
	}
	FVector Result;
	//                0~1사이의 비율이기 때문에
	Result.X = SpriteDataVector[Index].CuttingSize.X * TextureVector[Index]->GetTextureSize().X;
	Result.Y = SpriteDataVector[Index].CuttingSize.Y * TextureVector[Index]->GetTextureSize().Y;

	return Result;
}

UTexture* UPaperSprite::GetTexture(size_t Index)
{
	return TextureVector[Index];
}

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

std::shared_ptr<UPaperSprite> UPaperSprite::CreateSpriteToMeta(std::string_view _Name, std::string_view _DataFileExt)
{
	std::shared_ptr<UTexture> Tex = UTexture::Find<UTexture>(_Name);

	if (nullptr == Tex)
	{
		MSGASSERT("�������� �ʴ� �ؽ�ó�� ��������Ʈ�� ������� �����ϴ�.");
		return nullptr;
	}

	std::shared_ptr<UPaperSprite> NewRes = std::make_shared<UPaperSprite>();
	AddAsset<UPaperSprite>(NewRes, _Name, "");
	NewRes->Texture = Tex.get();

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
			std::string Number = UEngineString::InterString(Text, "x:", "\n", Start);
			SpriteData.Pivot.X = static_cast<float>(atof(Number.c_str()));
		}
		{
			std::string Number = UEngineString::InterString(Text, "y:", "\n", Start);
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

ID3D11ShaderResourceView* UPaperSprite::GetShaderResourceView()
{
	return Texture->GetShaderResourceView();
}

FPaperSpriteData UPaperSprite::GetSpriteData(size_t Index)
{
	if (SpriteDataVector.size() <= Index)
	{
		MSGASSERT("��������Ʈ�� �ε����� �ʰ��Ͽ� ����Ϸ��� �߽��ϴ�.");
	}

	return SpriteDataVector[Index];
}

FVector UPaperSprite::GetSpriteScaleToReal(size_t Index)
{
	if (SpriteDataVector.size() <= Index)
	{
		MSGASSERT("��������Ʈ�� �ε����� �ʰ��Ͽ� ����Ϸ��� �߽��ϴ�.");
	}
	FVector Result;
	//                0~1������ �����̱� ������
	Result.X = SpriteDataVector[Index].CuttingSize.X * Texture->GetTextureSize().X;
	Result.Y = SpriteDataVector[Index].CuttingSize.Y * Texture->GetTextureSize().Y;

	return Result;
}

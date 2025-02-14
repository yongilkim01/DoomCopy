#pragma once
#include "Engine/Classes/Engine/EngineResource.h"
#include "Core/Public/Math/EngineMath.h"
#include "Core/Public/Misc/DirectoryHelper.h"
#include "Engine/Public/Global/EngineDefine.h"
#include "Engine/Public/Global/EngineStruct.h"

class UTexture;

/**
 *	설명
 */
class UPaperSprite : public UResource
{
public:
	/** 생성자, 소멸자 */
	UPaperSprite();
	~UPaperSprite();

	/** 객체 값 복사 방지 */
	UPaperSprite(const UPaperSprite& Other) = delete;
	UPaperSprite(UPaperSprite&& Other) noexcept = delete;
	UPaperSprite& operator=(const UPaperSprite& Other) = delete;
	UPaperSprite& operator=(UPaperSprite&& Other) noexcept = delete;

	ENGINE_API static std::shared_ptr<UPaperSprite> CreateSpriteToFolder(std::string_view SpritePath)
	{
		FDirectoryHelper DirectoryHelper = SpritePath;

		return CreateSpriteToFolder(DirectoryHelper.GetDirectoryName(), SpritePath);
	}
	ENGINE_API static std::shared_ptr<UPaperSprite> CreateSpriteToFolder(std::string_view DirectoryName, std::string_view SpritePath);
	ENGINE_API static std::shared_ptr<UPaperSprite> CreateSpriteToCount(std::string_view Name, int X, int Y, FVector Pivot = FVector{ 0.5f, 0.5f });
	ENGINE_API static std::shared_ptr<UPaperSprite> CreateSpriteToMeta(std::string_view SpriteFileName, std::string_view SpriteFileExtension);
	ENGINE_API ID3D11ShaderResourceView* GetShaderResourceView(size_t Index = 0);
	ENGINE_API FPaperSpriteData GetSpriteData(size_t Index);
	ENGINE_API FVector GetSpriteScaleToReal(size_t Index);
	ENGINE_API UTexture* GetTexture(size_t Index = 0);

	ENGINE_API size_t GetSpriteCount()
	{
		return TextureVector.size();
	}


protected:

private:
	std::vector<UTexture*> TextureVector;
	std::vector<FPaperSpriteData> SpriteDataVector;
};
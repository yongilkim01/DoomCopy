#pragma once
#include "Engine/Classes/Engine/EngineResource.h"
#include "Core/Public/Math/EngineMath.h"
#include "Core/Public/Misc/DirectoryHelper.h"
#include "EngineDefine.h"

class UTexture;

struct FPaperSpriteData
{
	float4 CuttingLocation = { 0.0f, 0.0f };
	float4 CuttingSize = { 1.0f, 1.0f };
	float4 Pivot = { 0.5f, 0.5f };
};

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
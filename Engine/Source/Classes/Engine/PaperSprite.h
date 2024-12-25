#pragma once
#include "Classes/Engine/AssetManager.h"
#include "Core/Math/EngineMath.h"
#include "EngineDefine.h"

struct FPaperSpriteData
{
	float4 CuttingLocation = { 0.0f, 0.0f };
	float4 CuttingSize = { 1.0f, 1.0f };
};

/**
 *	설명
 */
class UPaperSprite : public UAssetManager
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

	ENGINE_API static std::shared_ptr<UPaperSprite> CreateSpriteToMeta(
		std::string_view SpriteFileName, std::string_view SpriteFileExtension);
	ENGINE_API ID3D11ShaderResourceView* GetShaderResourceView();
	ENGINE_API FPaperSpriteData GetSpriteData(size_t Index);

protected:

private:
	UTexture* Texture;
	std::vector<FPaperSpriteData> SpriteDataVector;
};
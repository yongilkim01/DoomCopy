#include "pch.h"
#include "UMG/Public/Components/Image.h"

#include "Engine/Classes/Engine/PaperSprite.h"

UImage::UImage()
{
	RenderAsset.SetTransformObject(this);
	RenderAsset.SetMesh("Rect");
	RenderAsset.SetMaterial("WidgetMaterial");

	RenderAsset.ConstantBufferLinkData("ResultColor", WidgetColor);
	RenderAsset.ConstantBufferLinkData("FSpriteData", WidgetData);
	RenderAsset.ConstantBufferLinkData("FUVValue", WidgetUV);

	WidgetUV.PlusUVValue = { 0.0f, 0.0f, 0.0f, 0.0f };

	WidgetData.CuttingLocation = { 0.0f, 0.0f };
	WidgetData.CuttingSize = { 1.0f, 1.0f };
	WidgetData.Pivot = { 0.5f, 0.5f };

	WidgetColor.PlusColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	WidgetColor.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

UImage::~UImage()
{
}

void UImage::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UWidget::Render(CameraComponent, DeltaTime);

	if (true == bAutoScale && nullptr != Sprite)
	{
		FVector SpriteScale = Sprite->GetSpriteScaleToReal(CurIndex);
		SpriteScale.Z = 1.0f;
		SetRelativeScale3D(SpriteScale * AutoScaleRatio);
	}

	UpdateCameraTransform(CameraComponent);

	RenderAsset.Render(CameraComponent, DeltaTime);
}

void UImage::SetSprite(std::string_view SpriteName, UINT Index)
{
	Sprite = UPaperSprite::Find<UPaperSprite>(SpriteName).get();

	if (nullptr == Sprite)
	{
		MSGASSERT("존재하지 않는 텍스처를 세팅하려고 했습니다.");
		return;
	}

	GetRenderAsset().SetTexture("ImageTexture", Sprite->GetTexture(Index)->GetName());

	WidgetData.CuttingLocation = Sprite->GetSpriteData(Index).CuttingLocation;
	WidgetData.CuttingSize = Sprite->GetSpriteData(Index).CuttingSize;
	WidgetData.Pivot = Sprite->GetSpriteData(Index).Pivot;
}

void UImage::SetTexture(std::string_view TextureName, bool bAutoScaleValue, float bRatioValue)
{
	std::shared_ptr<UTexture> Texture = UTexture::Find<UTexture>(TextureName);

	if (nullptr == Texture)
	{
		MSGASSERT("로드하지 않은 텍스처를 사용하려고 했습니다.");
	}

	GetRenderAsset().SetTexture("ImageTexture", TextureName);

	if (true == bAutoScale)
	{
		SetRelativeScale3D(Texture->GetTextureSize() * bRatioValue);
	}
}

#include "pch.h"
#include "UMG/Public/Components/Widget.h"
#include "Engine/Classes/GameFramework/HUD.h"
#include "Engine/Classes/Engine/PaperSprite.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Camera/CameraActor.h"
#include "Input/EngineInput.h"

UWidget::UWidget()
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

UWidget::~UWidget()
{
}

void UWidget::Tick(float DeltaTime)
{
	std::shared_ptr<ACameraActor> CameraActor = GetWorld()->GetCamera(EEngineCameraType::UICamera);
	
	FTransform Transform;

	Transform.WorldLocation = CameraActor->ScreenMouseLocationToWorldLocation();

	if (true == FTransform::PointToRect(Transform, GetComponentTransformRef()))
	{
		if (nullptr != HoverEventFunction)
		{
			HoverEventFunction();
		}
		if (true == UEngineInput::IsDown(VK_LBUTTON))
		{
			if (nullptr != MouseClickEventFunction)
			{
				MouseClickEventFunction();
			}
		}
		if (true == UEngineInput::IsUp(VK_LBUTTON))
		{
			if (nullptr != MouseUpEventFunction)
			{
				MouseUpEventFunction();
			}
		}
	}
}

void UWidget::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	if (true == bAutoScale && nullptr != Sprite)
	{
		FVector SpriteScale = Sprite->GetSpriteScaleToReal(CurIndex);
		SpriteScale.Z = 1.0f;
		SetRelativeScale3D(SpriteScale * AutoScaleRatio);
	}

	UpdateCameraTransform(CameraComponent);

	RenderAsset.Render(CameraComponent, DeltaTime);
}

void UWidget::SetSprite(std::string_view SpriteName, UINT Index/* = 0 */)
{
	Sprite = UPaperSprite::Find<UPaperSprite>(SpriteName).get();

	if (nullptr == Sprite)
	{
		MSGASSERT("존재하지 않는 텍스처를 세팅하려고 했습니다.");
		return;
	}

	GetRenderUnit().SetTexture("ImageTexture", Sprite->GetTexture(Index)->GetName());

	WidgetData.CuttingLocation = Sprite->GetSpriteData(Index).CuttingLocation;
	WidgetData.CuttingSize = Sprite->GetSpriteData(Index).CuttingSize;
	WidgetData.Pivot = Sprite->GetSpriteData(Index).Pivot;
}

void UWidget::SetTexture(std::string_view TextureName, bool bAutoScaleValue/* = false */, float bRatioValue/* = 1.0f */)
{
	std::shared_ptr<UTexture> Texture = UTexture::Find<UTexture>(TextureName);

	if (nullptr == Texture)
	{
		MSGASSERT("로드하지 않은 텍스처를 사용하려고 했습니다.");
	}

	GetRenderUnit().SetTexture("ImageTexture", TextureName);

	if (true == bAutoScale)
	{
		SetRelativeScale3D(Texture->GetTextureSize() * bRatioValue);
	}
}

ULevel* UWidget::GetWorld()
{
	return HUD->GetWorld();
}
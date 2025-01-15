#include "pch.h"
#include "UMG/Public/Components/Widget.h"
#include "Engine/Classes/GameFramework/HUD.h"

UWidget::UWidget()
{
    RenderAsset.SetTransformObject(this);
    RenderAsset.SetMesh("Rect");
    RenderAsset.SetMaterial("SpriteMaterial");

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

void UWidget::Render(UCameraComponent* CameraComponent, float DeltaTime)
{

}

ULevel* UWidget::GetWorld()
{
	return HUD->GetWorld();
}
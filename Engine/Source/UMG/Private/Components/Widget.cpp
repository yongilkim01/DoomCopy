#include "pch.h"
#include "UMG/Public/Components/Widget.h"
#include "Engine/Classes/GameFramework/HUD.h"

UWidget::UWidget()
{
    RenderUnit.SetMesh("Rect");
    RenderUnit.SetMaterial("SpriteMaterial");

    //RenderUnit.ConstantBufferLinkData()
}

UWidget::~UWidget()
{
}

void UWidget::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
}

ULevel* UWidget::GetWorld()
{
    return nullptr;
}

void UWidget::TransformUpdate()
{

}

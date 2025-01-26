#include "Engine.h"
#include "UI/Public/Widget/Widget.h"
#include "Engine/Classes/GameFramework/HUD.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Camera/CameraActor.h"
#include "Platform/Public/Input/EngineInput.h"

UWidget::UWidget()
{

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

}

ULevel* UWidget::GetWorld()
{
	return HUD->GetWorld();
}
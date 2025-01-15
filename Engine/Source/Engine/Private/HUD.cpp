#include "pch.h"
#include "Engine/Classes/GameFramework/HUD.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "UMG/Public/Components/Widget.h"

AHUD::AHUD()
{
}

AHUD::~AHUD()
{
}

void AHUD::RenderUI(UCameraComponent* CameraComponent, float DeltaTime)
{
	for (std::pair<const int, std::list<std::shared_ptr<UWidget>>> WidgetPair : WidgetMap)
	{
		std::list<std::shared_ptr<UWidget>>& WidgetList = WidgetPair.second;

		for (std::shared_ptr<UWidget> Widget : WidgetList)
		{
			if (false == Widget->IsActive())
			{
				continue;
			}

			Widget->Render(CameraComponent, DeltaTime);
		}
	}
}
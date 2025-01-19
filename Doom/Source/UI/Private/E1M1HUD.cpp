#include "pch.h"
#include "UI/Public/E1M1HUD.h"

#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/TextBox.h>

AE1M1HUD::AE1M1HUD()
{
}

AE1M1HUD::~AE1M1HUD()
{
}

void AE1M1HUD::BeginPlay()
{
	AHUD::BeginPlay();

	{
		std::shared_ptr<UImage> Widget = CreateWidget<UImage>(-1);
		Widget->SetWorldScale3D({ 100, 100, 1 });
		Widget->SetWorldLocation({ -200, 300 });
		Widget->SetTexture("BackGround.png");
		Widget->SetMouseClickEventFunction([]()
			{
				UEngineDebug::OutPutString("Click~~~~~~~~~");
			});
	}

	{
		std::shared_ptr<UTextBox> Widget = CreateWidget<UTextBox>(1);

		Widget->SetWorldLocation({ 200, 300 });
		Widget->SetFont("±√º≠");
		Widget->SetText("æ»≥Á«œººø‰");
	}
}

void AE1M1HUD::Tick(float DeltaTime)
{
	AHUD::Tick(DeltaTime);

}

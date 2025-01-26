#include "Doom.h"
#include "Public/UI/HUD/TitleHUD.h"

#include <UI/Public/Widget/Image.h>
#include <UI/Public/Widget/TextBox.h>

ATitleHUD::ATitleHUD()
{
}

ATitleHUD::~ATitleHUD()
{
}

void ATitleHUD::BeginPlay()
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

void ATitleHUD::Tick(float DeltaTime)
{
	AHUD::Tick(DeltaTime);

}

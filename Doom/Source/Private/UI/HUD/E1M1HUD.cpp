#include "Doom.h"
#include "Public/UI/HUD/E1M1HUD.h"

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
		Widget->SetWorldScale3D({ 1280, 128, 1 });
		Widget->SetWorldLocation({ 0.0f, -296.0f });
		Widget->SetTexture("StatusBar.png");
		Widget->SetMouseClickEventFunction([]()
			{
				//UEngineDebug::OutPutString("Click~~~~~~~~~");
			});
	}
	{
		std::shared_ptr<UImage> DooGuyWidget = CreateWidget<UImage>(-1);
		DooGuyWidget->SetWorldScale3D({ 110, 120, 1 });
		DooGuyWidget->SetWorldLocation({ -52.0f, -358.0f });
		DooGuyWidget->CreateAnimation("DoomGuy100", "DoomGuyHUD.png", 3, 5, 1.0f, true);
		DooGuyWidget->ChangeAnimation("DoomGuy100");
		DooGuyWidget->SetMouseClickEventFunction([]()
			{
				//UEngineDebug::OutPutString("Click~~~~~~~~~");
			});
	}
}

void AE1M1HUD::Tick(float DeltaTime)
{
	AHUD::Tick(DeltaTime);

}

#include "pch.h"
#include "UI/Public/TitleHUD.h"

#include <UMG/Public/Components/Widget.h>

ATitleHUD::ATitleHUD()
{
}

ATitleHUD::~ATitleHUD()
{
}

void ATitleHUD::BeginPlay()
{
	AHUD::BeginPlay();

	std::shared_ptr<UWidget> Button = CreateWidget<UWidget>(-1);
	Button->SetWorldScale3D({ 100, 100, 1 });
	Button->SetWorldLocation({ -200, 300 });
	Button->SetTexture("BackGround.png");
	Button->SetMouseClickEventFunction([]()
		{
			UEngineDebug::OutPutString("Click~~~~~~~~~");
		});
}

void ATitleHUD::Tick(float DeltaTime)
{
	AHUD::Tick(DeltaTime);

}

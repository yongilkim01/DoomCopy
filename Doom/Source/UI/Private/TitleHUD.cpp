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

	CreateWidget<UWidget>();
}

void ATitleHUD::Tick(float DeltaTime)
{
	AHUD::Tick(DeltaTime);

}

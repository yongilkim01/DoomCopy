#include "Doom.h"
#include "Public/UI/HUD/E1M1HUD.h"

#include <UI/Public/Widget/Image.h>
#include <UI/Public/Widget/TextBox.h>

#include "Public/Global/GameInstance/DoomGameInstance.h"

AE1M1HUD::AE1M1HUD()
{
}

AE1M1HUD::~AE1M1HUD()
{
}

void AE1M1HUD::BeginPlay()
{
	AHUD::BeginPlay();

	BeginPlayStateWidget();
	BeginPlayHealthWidget();
	BeginPlayAmmoWidget();
	BeginPlayArmorWidget();
	BeginPlayAmmoStateWidget();

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

	TickHealthWidget();
	TickAmmoWidget();
	TickArmorWidget();
	TickAmmoStateWidget();
}

std::vector<int> AE1M1HUD::SlitDigits(int Number)
{
	std::vector<int> Result;
	while (Number > 0)
	{
		Result.push_back(Number % 10); // 1의 자리부터 추가
		Number /= 10;
	}
	return Result;
}

void AE1M1HUD::BeginPlayStateWidget()
{
	std::shared_ptr<UImage> MainStateWidget = CreateWidget<UImage>(-1);
	MainStateWidget->SetWorldScale3D({ 1280, 128, 1 });
	MainStateWidget->SetWorldLocation({ 0.0f, -296.0f });
	MainStateWidget->SetTexture("StatusBar.png");

	std::shared_ptr<UImage> ArmsStateWidget = CreateWidget<UImage>(-1);
	ArmsStateWidget->SetWorldScale3D({ 160, 128, 1 });
	ArmsStateWidget->SetWorldLocation({ -144.0f, -296.0f });
	ArmsStateWidget->SetTexture("ArmsStatus.png");
}
void AE1M1HUD::TickStateWidget()
{
}

void AE1M1HUD::BeginPlayHealthWidget()
{
	int FontSize = 56;

	std::shared_ptr<UImage> HealthWidget100 = CreateWidget<UImage>(-1);
	HealthWidget100->SetWorldScale3D({ 56, 64, 1 });
	HealthWidget100->SetWorldLocation({ -419.0f, -278.0f });
	HealthWidget100->SetTexture("HUDFont_1.png");

	std::shared_ptr<UImage> HealthWidget10 = CreateWidget<UImage>(-1);
	HealthWidget10->SetWorldScale3D({ 56, 64, 1 });
	HealthWidget10->SetWorldLocation({ -419.0f + (FontSize * 1), -278.0f });
	HealthWidget10->SetTexture("HUDFont_0.png");

	std::shared_ptr<UImage> HealthWidget1 = CreateWidget<UImage>(-1);
	HealthWidget1->SetWorldScale3D({ 56, 64, 1 });
	HealthWidget1->SetWorldLocation({ -419.0f + (FontSize * 2), -278.0f });
	HealthWidget1->SetTexture("HUDFont_3.png");

	HealthWidgetVector.push_back(HealthWidget1);
	HealthWidgetVector.push_back(HealthWidget10);
	HealthWidgetVector.push_back(HealthWidget100);

	std::shared_ptr<UImage> HealthWidgetPercentege = CreateWidget<UImage>(-1);
	HealthWidgetPercentege->SetWorldScale3D({ 56, 64, 1 });
	HealthWidgetPercentege->SetWorldLocation({ -419.0f + (FontSize * 3), -278.0f });
	HealthWidgetPercentege->SetTexture("HUDFont_P.png");

}

void AE1M1HUD::TickHealthWidget()
{
	int DoomGuyHealth = GetGameInstance<UDoomGameInstance>()->GetDoomGuyHealth();

	std::vector<int> SlitDoomGuyHealth = SlitDigits(DoomGuyHealth);

	if (SlitDoomGuyHealth.size() == 1)
	{
		HealthWidgetVector[1]->SetActive(false);
		HealthWidgetVector[2]->SetActive(false);
	}
	else if (SlitDoomGuyHealth.size() == 2)
	{
		HealthWidgetVector[2]->SetActive(false);
	}
	else
	{
		HealthWidgetVector[1]->SetActive(true);
		HealthWidgetVector[2]->SetActive(true);
	}

	for (int i = 0; i < SlitDoomGuyHealth.size(); i++)
	{
		std::string Font = "HUDFont_" + std::to_string(SlitDoomGuyHealth[i]) + ".png";
		HealthWidgetVector[i]->SetTexture(Font);
	}
}

void AE1M1HUD::BeginPlayAmmoWidget()
{
	int FontSize = 56;
	float InitX = -602.0f;
	float InitY = -278.0f;

	std::shared_ptr<UImage> AmmoWidget100 = CreateWidget<UImage>(-1);
	AmmoWidget100->SetWorldScale3D({ 56, 64, 1 });
	AmmoWidget100->SetWorldLocation({ InitX, InitY });
	AmmoWidget100->SetTexture("HUDFont_1.png");

	std::shared_ptr<UImage> AmmoWidget10 = CreateWidget<UImage>(-1);
	AmmoWidget10->SetWorldScale3D({ 56, 64, 1 });
	AmmoWidget10->SetWorldLocation({ InitX + (FontSize * 1), InitY });
	AmmoWidget10->SetTexture("HUDFont_0.png");

	std::shared_ptr<UImage> AmmoWidget1 = CreateWidget<UImage>(-1);
	AmmoWidget1->SetWorldScale3D({ 56, 64, 1 });
	AmmoWidget1->SetWorldLocation({ InitX + (FontSize * 2), InitY });
	AmmoWidget1->SetTexture("HUDFont_3.png");

	AmmoWidgetVector.push_back(AmmoWidget1);
	AmmoWidgetVector.push_back(AmmoWidget10);
	AmmoWidgetVector.push_back(AmmoWidget100);

}

void AE1M1HUD::TickAmmoWidget()
{
	int DoomGuyAmmo = GetGameInstance<UDoomGameInstance>()->GetDoomGuyBullet();

	std::vector<int> SlitDoomGuyAmmo = SlitDigits(DoomGuyAmmo);

	if (SlitDoomGuyAmmo.size() == 1)
	{
		AmmoWidgetVector[1]->SetActive(false);
		AmmoWidgetVector[2]->SetActive(false);
	}
	else if (SlitDoomGuyAmmo.size() == 2)
	{
		AmmoWidgetVector[2]->SetActive(false);
	}
	else
	{
		AmmoWidgetVector[1]->SetActive(true);
		AmmoWidgetVector[2]->SetActive(true);
	}

	for (int i = 0; i < SlitDoomGuyAmmo.size(); i++)
	{
		std::string Font = "HUDFont_" + std::to_string(SlitDoomGuyAmmo[i]) + ".png";
		AmmoWidgetVector[i]->SetTexture(Font);
	}
}

void AE1M1HUD::BeginPlayArmorWidget()
{
	int FontSize = 56;
	float InitX = 104.0f;
	float InitY = -278.0f;

	std::shared_ptr<UImage> ArmorWidget100 = CreateWidget<UImage>(-1);
	ArmorWidget100->SetWorldScale3D({ 56, 64, 1 });
	ArmorWidget100->SetWorldLocation({ InitX, InitY });
	ArmorWidget100->SetTexture("HUDFont_1.png");

	std::shared_ptr<UImage> ArmorWidget10 = CreateWidget<UImage>(-1);
	ArmorWidget10->SetWorldScale3D({ 56, 64, 1 });
	ArmorWidget10->SetWorldLocation({ InitX + (FontSize * 1), InitY });
	ArmorWidget10->SetTexture("HUDFont_0.png");

	std::shared_ptr<UImage> ArmorWidget1 = CreateWidget<UImage>(-1);
	ArmorWidget1->SetWorldScale3D({ 56, 64, 1 });
	ArmorWidget1->SetWorldLocation({ InitX + (FontSize * 2), InitY });
	ArmorWidget1->SetTexture("HUDFont_3.png");

	ArmorWidgetVector.push_back(ArmorWidget1);
	ArmorWidgetVector.push_back(ArmorWidget10);
	ArmorWidgetVector.push_back(ArmorWidget100);

	std::shared_ptr<UImage> ArmorWidgetPercentage = CreateWidget<UImage>(-1);
	ArmorWidgetPercentage->SetWorldScale3D({ 56, 64, 1 });
	ArmorWidgetPercentage->SetWorldLocation({ InitX + (FontSize * 3), InitY });
	ArmorWidgetPercentage->SetTexture("HUDFont_P.png");
}

void AE1M1HUD::TickArmorWidget()
{
	int DoomGuyArmor = GetGameInstance<UDoomGameInstance>()->GetDoomGuyArmor();

	std::vector<int> SlitDoomGuyArmor = SlitDigits(DoomGuyArmor);

	if (SlitDoomGuyArmor.size() == 0)
	{
		ArmorWidgetVector[0]->SetTexture("HUDFont_0.png");
		ArmorWidgetVector[1]->SetActive(false);
		ArmorWidgetVector[2]->SetActive(false);
	}
	else
	{
		if (SlitDoomGuyArmor.size() == 1)
		{
			ArmorWidgetVector[1]->SetActive(false);
			ArmorWidgetVector[2]->SetActive(false);
		}
		else if (SlitDoomGuyArmor.size() == 2)
		{
			ArmorWidgetVector[2]->SetActive(false);
		}
		else
		{
			ArmorWidgetVector[1]->SetActive(true);
			ArmorWidgetVector[2]->SetActive(true);
		}

		for (int i = 0; i < SlitDoomGuyArmor.size(); i++)
		{
			std::string Font = "HUDFont_" + std::to_string(SlitDoomGuyArmor[i]) + ".png";
			ArmorWidgetVector[i]->SetTexture(Font);
		}
	}
}

void AE1M1HUD::BeginPlayAmmoStateWidget()
{
	int PaddingX = 16;
	int PaddingY = 24;
	float InitX = 592.0f;
	float InitY = -263.0f;

	std::shared_ptr<UImage> AmmoMaxBullet = CreateWidget<UImage>(-1);
	AmmoMaxBullet->SetWorldScale3D({ 48, 24, 1 });
	AmmoMaxBullet->SetWorldLocation({ InitX, InitY - (PaddingY * 0) });
	AmmoMaxBullet->SetTexture("HUDMaxBullet.png");

	std::shared_ptr<UImage> AmmoMaxShell = CreateWidget<UImage>(-1);
	AmmoMaxShell->SetWorldScale3D({ 48, 24, 1 });
	AmmoMaxShell->SetWorldLocation({ InitX, InitY - (PaddingY * 1) });
	AmmoMaxShell->SetTexture("HUDMaxShell.png");

	std::shared_ptr<UImage> AmmoMaxRocket = CreateWidget<UImage>(-1);
	AmmoMaxRocket->SetWorldScale3D({ 48, 24, 1 });
	AmmoMaxRocket->SetWorldLocation({ InitX, InitY - (PaddingY * 2) });
	AmmoMaxRocket->SetTexture("HUDMaxShell.png");

	std::shared_ptr<UImage> AmmoMaxCell = CreateWidget<UImage>(-1);
	AmmoMaxCell->SetWorldScale3D({ 48, 24, 1 });
	AmmoMaxCell->SetWorldLocation({ InitX, InitY - (PaddingY * 3) });
	AmmoMaxCell->SetTexture("HUDMaxCell.png");

	float CurStateInitX = 472.0f;

	{
		std::shared_ptr<UImage> AmmoCurBullet100 = CreateWidget<UImage>(-1);
		AmmoCurBullet100->SetWorldScale3D({ 16, 24, 1 });
		AmmoCurBullet100->SetWorldLocation({ CurStateInitX + (PaddingX * 0), InitY - (PaddingY * 0) });
		AmmoCurBullet100->SetTexture("HUDSmallFont_0.png");

		std::shared_ptr<UImage> AmmoCurBullet10 = CreateWidget<UImage>(-1);
		AmmoCurBullet10->SetWorldScale3D({ 16, 24, 1 });
		AmmoCurBullet10->SetWorldLocation({ CurStateInitX + (PaddingX * 1), InitY - (PaddingY * 0) });
		AmmoCurBullet10->SetTexture("HUDSmallFont_1.png");

		std::shared_ptr<UImage> AmmoCurBullet1 = CreateWidget<UImage>(-1);
		AmmoCurBullet1->SetWorldScale3D({ 16, 24, 1 });
		AmmoCurBullet1->SetWorldLocation({ CurStateInitX + (PaddingX * 2), InitY - (PaddingY * 0) });
		AmmoCurBullet1->SetTexture("HUDSmallFont_2.png");

		CurBulletWidgetVector.push_back(AmmoCurBullet1);
		CurBulletWidgetVector.push_back(AmmoCurBullet10);
		CurBulletWidgetVector.push_back(AmmoCurBullet100);
	}
	{
		std::shared_ptr<UImage> AmmoCurShell10 = CreateWidget<UImage>(-1);
		AmmoCurShell10->SetWorldScale3D({ 16, 24, 1 });
		AmmoCurShell10->SetWorldLocation({ CurStateInitX + (PaddingX * 1), InitY - (PaddingY * 1) });
		AmmoCurShell10->SetTexture("HUDSmallFont_1.png");

		std::shared_ptr<UImage> AmmoCurShell1 = CreateWidget<UImage>(-1);
		AmmoCurShell1->SetWorldScale3D({ 16, 24, 1 });
		AmmoCurShell1->SetWorldLocation({ CurStateInitX + (PaddingX * 2), InitY - (PaddingY * 1) });
		AmmoCurShell1->SetTexture("HUDSmallFont_2.png");

		CurShellWidgetVector.push_back(AmmoCurShell1);
		CurShellWidgetVector.push_back(AmmoCurShell10);
	}
	{
		std::shared_ptr<UImage> AmmoCurRocket1 = CreateWidget<UImage>(-1);
		AmmoCurRocket1->SetWorldScale3D({ 16, 24, 1 });
		AmmoCurRocket1->SetWorldLocation({ CurStateInitX + (PaddingX * 2), InitY - (PaddingY * 2) });
		AmmoCurRocket1->SetTexture("HUDSmallFont_0.png");
	}
	{
		std::shared_ptr<UImage> AmmoCurCell1 = CreateWidget<UImage>(-1);
		AmmoCurCell1->SetWorldScale3D({ 16, 24, 1 });
		AmmoCurCell1->SetWorldLocation({ CurStateInitX + (PaddingX * 2), InitY - (PaddingY * 3) });
		AmmoCurCell1->SetTexture("HUDSmallFont_0.png");
	}
}

void AE1M1HUD::TickAmmoStateWidget()
{
	{
		int DoomCurBullet = GetGameInstance<UDoomGameInstance>()->GetDoomGuyBullet();

		std::vector<int> SlitDoomGuyBullet = SlitDigits(DoomCurBullet);

		if (SlitDoomGuyBullet.size() == 0)
		{
			CurBulletWidgetVector[0]->SetTexture("HUDSmallFont_0.png");
			CurBulletWidgetVector[1]->SetActive(false);
			CurBulletWidgetVector[2]->SetActive(false);
		}
		else
		{
			if (SlitDoomGuyBullet.size() == 1)
			{
				CurBulletWidgetVector[1]->SetActive(false);
				CurBulletWidgetVector[2]->SetActive(false);
			}
			else if (SlitDoomGuyBullet.size() == 2)
			{
				CurBulletWidgetVector[2]->SetActive(false);
			}
			else
			{
				CurBulletWidgetVector[1]->SetActive(true);
				CurBulletWidgetVector[2]->SetActive(true);
			}

			for (int i = 0; i < SlitDoomGuyBullet.size(); i++)
			{
				std::string Font = "HUDSmallFont_" + std::to_string(SlitDoomGuyBullet[i]) + ".png";
				CurBulletWidgetVector[i]->SetTexture(Font);
			}
		}
	}
	{
		int DoomCurShell = GetGameInstance<UDoomGameInstance>()->GetDoomGuyShell();

		std::vector<int> SlitDoomGuyShell = SlitDigits(DoomCurShell);

		if (SlitDoomGuyShell.size() == 0)
		{
			CurShellWidgetVector[0]->SetTexture("HUDSmallFont_0.png");
			CurShellWidgetVector[1]->SetActive(false);
		}
		else
		{
			if (SlitDoomGuyShell.size() == 1)
			{
				CurShellWidgetVector[1]->SetActive(false);
			}
			else
			{
				CurShellWidgetVector[1]->SetActive(true);
			}

			for (int i = 0; i < SlitDoomGuyShell.size(); i++)
			{
				std::string Font = "HUDSmallFont_" + std::to_string(SlitDoomGuyShell[i]) + ".png";
				CurShellWidgetVector[i]->SetTexture(Font);
			}
		}
	}
}

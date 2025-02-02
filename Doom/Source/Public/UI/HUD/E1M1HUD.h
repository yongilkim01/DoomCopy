#pragma once
#include <Engine/Classes/GameFramework/HUD.h>

class UImage;

class AE1M1HUD : public AHUD
{
public:
	/** 생성자, 소멸자 */
	AE1M1HUD();
	~AE1M1HUD();

	/** 객체 값 복사 방지 */
	AE1M1HUD(const AE1M1HUD& Other) = delete;
	AE1M1HUD(AE1M1HUD&& Other) noexcept = delete;
	AE1M1HUD& operator=(const AE1M1HUD& Other) = delete;
	AE1M1HUD& operator=(AE1M1HUD&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	std::vector<int> SlitDigits(int Number);

	void BeginPlayStateWidget();
	void TickStateWidget();

	void BeginPlayHealthWidget();
	void TickHealthWidget();

	void BeginPlayAmmoWidget();
	void TickAmmoWidget();

	void BeginPlayArmorWidget();
	void TickArmorWidget();

	void BeginPlayAmmoStateWidget();
	void TickAmmoStateWidget();

protected:

private:
	std::vector<std::shared_ptr<UImage>> HealthWidgetVector;
	std::vector<std::shared_ptr<UImage>> AmmoWidgetVector;
	std::vector<std::shared_ptr<UImage>> ArmorWidgetVector;

	std::vector<std::shared_ptr<UImage>> CurBulletWidgetVector;
	std::vector<std::shared_ptr<UImage>> CurShellWidgetVector;

	std::vector<std::shared_ptr<UImage>> StateWidgetVector;

};
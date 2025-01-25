#pragma once
#include <Engine/Classes/GameFramework/HUD.h>

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

protected:

private:

};
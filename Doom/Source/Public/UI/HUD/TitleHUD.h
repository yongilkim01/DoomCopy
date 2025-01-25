#pragma once
#include <Engine/Classes/GameFramework/HUD.h>

class ATitleHUD : public AHUD
{
public:
	/** 생성자, 소멸자 */
	ATitleHUD();
	~ATitleHUD();

	/** 객체 값 복사 방지 */
	ATitleHUD(const ATitleHUD& Other) = delete;
	ATitleHUD(ATitleHUD&& Other) noexcept = delete;
	ATitleHUD& operator=(const ATitleHUD& Other) = delete;
	ATitleHUD& operator=(ATitleHUD&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

private:

};
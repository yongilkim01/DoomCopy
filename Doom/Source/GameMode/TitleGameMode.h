#pragma once
#include "Engine/Classes/GameFramework/GameMode.h"

class ATitleLogo;

/**
 *	Ÿ��Ʋ ���Ӹ�� Ŭ����
 */
class ATitleGameMode : public AGameMode
{
public:
	/** ������, �Ҹ��� */
	ATitleGameMode();
	~ATitleGameMode();

	/** ��ü �� ���� ���� */
	ATitleGameMode(const ATitleGameMode& Other) = delete;
	ATitleGameMode(ATitleGameMode&& Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& Other) noexcept = delete;

	void Tick(float DeltaTime);

protected:

private:
	std::shared_ptr<ATitleLogo> TitleLogo;

};
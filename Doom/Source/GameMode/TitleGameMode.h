#pragma once
#include "GameFramework/Actor.h"

/**
 *	Ÿ��Ʋ ���Ӹ�� Ŭ����
 */
class ATitleGameMode : public AActor
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

};
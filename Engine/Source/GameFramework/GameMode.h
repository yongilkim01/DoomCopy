#pragma once
#include "GameFramework/Actor.h"

/**
 *	����
 */
class AGameMode : public AActor
{
public:
	/** ������, �Ҹ��� */
	AGameMode();
	~AGameMode();

	/** ��ü �� ���� ���� */
	AGameMode(const AGameMode& Other) = delete;
	AGameMode(AGameMode&& Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& Other) = delete;
	AGameMode& operator=(AGameMode&& Other) noexcept = delete;

protected:

private:

};


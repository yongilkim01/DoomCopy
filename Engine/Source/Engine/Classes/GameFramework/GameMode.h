#pragma once
#include "Engine/Classes/GameFramework/Actor.h"

/**
 *	����
 */
class AGameMode : public AActor
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API AGameMode();
	ENGINE_API ~AGameMode();

	/** ��ü �� ���� ���� */
	AGameMode(const AGameMode& Other) = delete;
	AGameMode(AGameMode&& Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& Other) = delete;
	AGameMode& operator=(AGameMode&& Other) noexcept = delete;

protected:

private:

};


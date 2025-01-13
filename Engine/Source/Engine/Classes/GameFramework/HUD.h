#pragma once
#include "Engine/Classes/GameFramework/Actor.h"

/**
 *	����
 */
class AHUD : public AActor
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API AHUD();
	ENGINE_API ~AHUD();

	/** ��ü �� ���� ���� */
	AHUD(const AHUD& Other) = delete;
	AHUD(AHUD&& Other) noexcept = delete;
	AHUD& operator=(const AHUD& Other) = delete;
	AHUD& operator=(AHUD&& Other) noexcept = delete;

	void CreateWidget();

protected:

private:

};


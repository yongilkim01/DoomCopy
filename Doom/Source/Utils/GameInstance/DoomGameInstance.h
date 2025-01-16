#pragma once
#include <Engine/Classes/Engine/GameInstance.h>
#include "Global/DoomStructs.h"

/**
 *	����
 */
class UDoomGameInstance : public UGameInstance
{
public:
	/** ������, �Ҹ��� */
	UDoomGameInstance();
	~UDoomGameInstance();

	/** ��ü �� ���� ���� */
	UDoomGameInstance(const UDoomGameInstance& Other) = delete;
	UDoomGameInstance(UDoomGameInstance&& Other) noexcept = delete;
	UDoomGameInstance& operator=(const UDoomGameInstance& Other) = delete;
	UDoomGameInstance& operator=(UDoomGameInstance&& Other) noexcept = delete;

	FDoomStatus DoomGuyStatus;

protected:

private:

};


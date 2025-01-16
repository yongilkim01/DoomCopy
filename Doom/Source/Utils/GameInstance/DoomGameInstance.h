#pragma once
#include <Engine/Classes/Engine/GameInstance.h>
#include "Global/DoomStructs.h"

/**
 *	설명
 */
class UDoomGameInstance : public UGameInstance
{
public:
	/** 생성자, 소멸자 */
	UDoomGameInstance();
	~UDoomGameInstance();

	/** 객체 값 복사 방지 */
	UDoomGameInstance(const UDoomGameInstance& Other) = delete;
	UDoomGameInstance(UDoomGameInstance&& Other) noexcept = delete;
	UDoomGameInstance& operator=(const UDoomGameInstance& Other) = delete;
	UDoomGameInstance& operator=(UDoomGameInstance&& Other) noexcept = delete;

	FDoomStatus DoomGuyStatus;

protected:

private:

};


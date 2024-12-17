#pragma once
#include "GameFramework/Actor.h"

/**
 *	설명
 */
class AGameMode : public AActor
{
public:
	/** 생성자, 소멸자 */
	AGameMode();
	~AGameMode();

	/** 객체 값 복사 방지 */
	AGameMode(const AGameMode& Other) = delete;
	AGameMode(AGameMode&& Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& Other) = delete;
	AGameMode& operator=(AGameMode&& Other) noexcept = delete;

protected:

private:

};


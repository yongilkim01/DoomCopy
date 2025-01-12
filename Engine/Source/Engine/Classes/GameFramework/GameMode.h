#pragma once
#include "Engine/Classes/GameFramework/Actor.h"

/**
 *	설명
 */
class AGameMode : public AActor
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API AGameMode();
	ENGINE_API ~AGameMode();

	/** 객체 값 복사 방지 */
	AGameMode(const AGameMode& Other) = delete;
	AGameMode(AGameMode&& Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& Other) = delete;
	AGameMode& operator=(AGameMode&& Other) noexcept = delete;

protected:

private:

};


#pragma once
#include "Engine/Classes/GameFramework/Actor.h"

/**
 *	설명
 */
class AHUD : public AActor
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API AHUD();
	ENGINE_API ~AHUD();

	/** 객체 값 복사 방지 */
	AHUD(const AHUD& Other) = delete;
	AHUD(AHUD&& Other) noexcept = delete;
	AHUD& operator=(const AHUD& Other) = delete;
	AHUD& operator=(AHUD&& Other) noexcept = delete;

	void CreateWidget();

protected:

private:

};


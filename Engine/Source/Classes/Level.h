#pragma once

#include "Core/Object/Object.h"
#include "EngineDefine.h"

/**
 *	엔진 레벨 클래스
 */
class ULevel : public UObject
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API ULevel();
	ENGINE_API ~ULevel();

	/** 객체 값 복사 방지 */
	ULevel(const ULevel& Other) = delete;
	ULevel(ULevel&& Other) noexcept = delete;
	ULevel& operator=(const ULevel& Other) = delete;
	ULevel& operator=(ULevel&& Other) noexcept = delete;

protected:

private:

};
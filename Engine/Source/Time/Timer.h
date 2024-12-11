#pragma once
#include <Windows.h>

/**
 *	엔진 시간 클래스
 */
class UEngineTimer
{
public:
	/** 생성자, 소멸자 */
	UEngineTimer();
	~UEngineTimer();

	/** 객체 값 복사 방지 */
	UEngineTimer(const UEngineTimer& Other) = delete;
	UEngineTimer(UEngineTimer&& Other) noexcept = delete;
	UEngineTimer& operator=(const UEngineTimer& Other) = delete;
	UEngineTimer& operator=(UEngineTimer&& Other) noexcept = delete;

	void TimeCheck();
	float GetDeltaTime()
	{
		return fDeltaTime;
	}
	double GetDoubleDeltaTime()
	{
		return DeltaTime;
	}
	void TimeStart();
	float End();
	double DEnd();

protected:

private:
	LARGE_INTEGER Count = LARGE_INTEGER();
	LARGE_INTEGER PrevTime = LARGE_INTEGER();
	LARGE_INTEGER CurTime = LARGE_INTEGER();

	double TimeCounter = 0.0f;
	double DeltaTime = 0.0;
	float fDeltaTime = 0.0f;
};


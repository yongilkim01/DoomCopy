#include "pch.h"
#include "Object.h"

UObject::UObject()
{
}

UObject::~UObject()
{
}

void UObject::Destroy(float Time)
{
	// 시간 설정
	DeathTime = Time;

	// 파괴까지 시간이 0초보다 클 경우 시간 검사
	if (0.0f < Time)
	{
		IsDeathTimeCheck = true;
		return;
	}

	IsDestroyValue = true;
}

void UObject::ReleaseTimeCheck(float DeltaTime)
{
	if (false == IsDeathTimeCheck)
	{
		return;
	}

	CurDeathTime += DeltaTime;

	if (DeathTime <= CurDeathTime)
	{
		IsDestroyValue = true;
	}
}

void UObject::ReleaseCheck(float DeltaTime)
{
}

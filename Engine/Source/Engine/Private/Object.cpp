#include "pch.h"
#include "Engine/Classes/Engine/Object.h"

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
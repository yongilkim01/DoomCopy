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
	// �ð� ����
	DeathTime = Time;

	// �ı����� �ð��� 0�ʺ��� Ŭ ��� �ð� �˻�
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

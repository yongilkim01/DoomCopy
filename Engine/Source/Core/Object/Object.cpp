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
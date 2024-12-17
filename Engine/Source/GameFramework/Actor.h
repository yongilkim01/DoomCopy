#pragma once
#include "Core/Object/Object.h"

class AActor : public UObject
{
public:
	/** ������, �Ҹ��� */
	AActor();
	~AActor();

	/** ��ü �� ���� ���� */
	AActor(const AActor& Other) = delete;
	AActor(AActor&& Other) noexcept = delete;
	AActor& operator=(const AActor& Other) = delete;
	AActor& operator=(AActor&& Other) noexcept = delete;

	void CreateDefaultSubObject() {}
};
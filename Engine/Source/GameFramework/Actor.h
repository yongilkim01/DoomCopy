#pragma once
#include "Core/Object/Object.h"

class AActor : public UObject
{
public:
	/** 생성자, 소멸자 */
	AActor();
	~AActor();

	/** 객체 값 복사 방지 */
	AActor(const AActor& Other) = delete;
	AActor(AActor&& Other) noexcept = delete;
	AActor& operator=(const AActor& Other) = delete;
	AActor& operator=(AActor&& Other) noexcept = delete;

	void CreateDefaultSubObject() {}
};
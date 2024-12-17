#pragma once
#include "Core/Object/Object.h"

class AActor;

/**
 *	설명
 */
class UActorComponent : public UObject
{
public:
	/** 생성자, 소멸자 */
	UActorComponent();
	~UActorComponent();

	/** 객체 값 복사 방지 */
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	std::shared_ptr<AActor> GetActor() {}

protected:

private:
	AActor* Owner;

};


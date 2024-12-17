#pragma once
#include "Core/Object/Object.h"

class AActor;

/**
 *	설명
 */
class UActorComponent : public UObject
{
	friend AActor;

public:
	/** 생성자, 소멸자 */
	UActorComponent();
	~UActorComponent();

	/** 객체 값 복사 방지 */
	UActorComponent(const UActorComponent& Other) = delete;
	UActorComponent(UActorComponent&& Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& Other) = delete;
	UActorComponent& operator=(UActorComponent&& Other) noexcept = delete;

	ENGINE_API virtual void InitializeComponent() {}
	ENGINE_API virtual void BeginPlay() {}

	AActor* GetOwner()
	{
		return Owner;
	}
	void SetOwner(AActor* NewOwner)
	{
		Owner = NewOwner;
	}

protected:

private:
	AActor* Owner;

};
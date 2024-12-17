#pragma once
#include "Core/Object/Object.h"
#include <memory>

class AActor;

/**
 *	����
 */
class UActorComponent : public UObject
{
public:
	/** ������, �Ҹ��� */
	UActorComponent();
	~UActorComponent();

	/** ��ü �� ���� ���� */
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	std::shared_ptr<AActor> GetActor() {}

	void SetOwner(AActor* NewOwner)
	{
		Owner = NewOwner;
	}

protected:

private:
	AActor* Owner;

};


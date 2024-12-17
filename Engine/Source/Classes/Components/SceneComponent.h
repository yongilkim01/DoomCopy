#pragma once
#include "Classes/Components/ActorComponent.h"

/**
 *	����
 */
class USceneComponent : public UActorComponent
{
public:
	/** ������, �Ҹ��� */
	USceneComponent();
	~USceneComponent();

	/** ��ü �� ���� ���� */
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

protected:

private:

};


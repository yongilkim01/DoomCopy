#pragma once
#include "SceneComponent.h"

/**
 *	����
 */
class UPhsyicsComponent : public USceneComponent
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UPhsyicsComponent();
	ENGINE_API ~UPhsyicsComponent();

	/** ��ü �� ���� ���� */
	UPhsyicsComponent(const UPhsyicsComponent& Other) = delete;
	UPhsyicsComponent(UPhsyicsComponent&& Other) noexcept = delete;
	UPhsyicsComponent& operator=(const UPhsyicsComponent& Other) = delete;
	UPhsyicsComponent& operator=(UPhsyicsComponent&& Other) noexcept = delete;

private:

};


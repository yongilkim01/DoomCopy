#pragma once
#include "SceneComponent.h"

/**
 *	설명
 */
class UPhsyicsComponent : public USceneComponent
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UPhsyicsComponent();
	ENGINE_API ~UPhsyicsComponent();

	/** 객체 값 복사 방지 */
	UPhsyicsComponent(const UPhsyicsComponent& Other) = delete;
	UPhsyicsComponent(UPhsyicsComponent&& Other) noexcept = delete;
	UPhsyicsComponent& operator=(const UPhsyicsComponent& Other) = delete;
	UPhsyicsComponent& operator=(UPhsyicsComponent&& Other) noexcept = delete;

private:

};


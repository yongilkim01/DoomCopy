#pragma once
#include "Classes/Components/ActorComponent.h"

/**
 *	설명
 */
class USceneComponent : public UActorComponent
{
public:
	/** 생성자, 소멸자 */
	USceneComponent();
	~USceneComponent();

	/** 객체 값 복사 방지 */
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

protected:

private:

};


#pragma once
#include "Engine/Classes/Components/ActorComponent.h"
#include "Engine/Classes/Engine/TransformObject.h"
#include "Core/Public/Math/EngineMath.h"

class AActor;

/**
 *	설명
 */
class USceneComponent : public UActorComponent, public UTransformObject
{
	friend AActor;
public:
	/** 생성자, 소멸자 */
	ENGINE_API USceneComponent();
	ENGINE_API ~USceneComponent();

	/** 객체 값 복사 방지 */
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	ENGINE_API virtual void TickComponent(float DeltaTime) override;

protected:
	ENGINE_API virtual void BeginPlay() override;

private:

};


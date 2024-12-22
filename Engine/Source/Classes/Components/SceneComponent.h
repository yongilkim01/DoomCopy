#pragma once
#include "Classes/Components/ActorComponent.h"
#include "Core/Math/EngineMath.h"

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

	/** 트랜스폼 관련 메소드 */
	void SetRealativeScale3D(const FVector& NewScale)
	{
		Transform.Scale = NewScale;
		Transform.TransformUpdate();

	}
	void SetWorldLocation(const FVector& NewLocation)
	{
		Transform.Location = NewLocation;
		Transform.TransformUpdate();
	}
	void AddWorldOffset(FVector DeltaLoaction)
	{
		Transform.Location += DeltaLoaction;
		Transform.TransformUpdate();
	}

	/** 겟, 셋 메소드 */
	FTransform GetComponentTransform()
	{
		return Transform;
	}
	FTransform& GetComponentTransformRef()
	{
		return Transform;
	}

protected:
	FTransform Transform;

private:

	USceneComponent* Parent;
	std::list<std::shared_ptr<USceneComponent>> Childs;
};


#pragma once
#include "Classes/Components/ActorComponent.h"
#include "Core/Math/EngineMath.h"

class AActor;

/**
 *	설명
 */
class USceneComponent : public UActorComponent
{
	friend AActor;
public:
	/** 생성자, 소멸자 */
	USceneComponent();
	~USceneComponent();

	/** 객체 값 복사 방지 */
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	ENGINE_API void TransformUpdate();
	ENGINE_API void SetupAttachment(std::shared_ptr<USceneComponent> ParentComponent);
	void SetupAttachment(USceneComponent* ParentComponent);

	/** 트랜스폼 관련 메소드 */
	void SetRelativeScale3D(const FVector& NewScale)
	{
		Transform.Scale = NewScale;
		TransformUpdate();

	}
	void SetWorldLocation(const FVector& NewLocation)
	{
		Transform.Location = NewLocation;
		TransformUpdate();
	}
	void AddWorldOffset(FVector DeltaLoaction)
	{
		Transform.Location += DeltaLoaction;
		TransformUpdate();
	}
	void SetWorldRotation(const FVector& NewRotation)
	{
		Transform.Rotation = NewRotation;
		TransformUpdate();
	}
	void AddWorldRotation(const FVector& DeltaRotation)
	{
		Transform.Rotation += DeltaRotation;
		TransformUpdate();
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
	ENGINE_API void BeginPlay() override;
	void ParentMatrixCheck();

	FTransform Transform;

private:

	USceneComponent* Parent = nullptr;
	std::list<std::shared_ptr<USceneComponent>> Childs;
};


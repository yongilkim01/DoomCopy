#pragma once
#include "Classes/Components/ActorComponent.h"
#include "Core/Math/EngineMath.h"

class AActor;

/**
 *	����
 */
class USceneComponent : public UActorComponent
{
	friend AActor;
public:
	/** ������, �Ҹ��� */
	USceneComponent();
	~USceneComponent();

	/** ��ü �� ���� ���� */
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	ENGINE_API void TransformUpdate();
	ENGINE_API void SetupAttachment(std::shared_ptr<USceneComponent> ParentComponent);
	void SetupAttachment(USceneComponent* ParentComponent);

	/** Ʈ������ ���� �޼ҵ� */
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

	/** ��, �� �޼ҵ� */
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


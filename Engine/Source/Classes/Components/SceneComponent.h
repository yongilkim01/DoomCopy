#pragma once
#include "Classes/Components/ActorComponent.h"
#include "Core/Math/EngineMath.h"

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

	/** Ʈ������ ���� �޼ҵ� */
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
	FTransform Transform;

private:

	USceneComponent* Parent;
	std::list<std::shared_ptr<USceneComponent>> Childs;
};


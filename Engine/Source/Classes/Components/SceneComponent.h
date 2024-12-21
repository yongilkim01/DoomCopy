#pragma once
#include "Classes/Components/ActorComponent.h"
#include "Math/EngineMath.h"

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

	/** ��, �� �޼ҵ� */
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


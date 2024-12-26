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
	ENGINE_API USceneComponent();
	ENGINE_API ~USceneComponent();

	/** ��ü �� ���� ���� */
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	ENGINE_API void TransformUpdate();
	ENGINE_API void SetupAttachment(std::shared_ptr<USceneComponent> ParentComponent);
	void SetupAttachment(USceneComponent* ParentComponent);

	/**
	 * �θ� �������� ������Ʈ�� ��� ��ġ�� �����ϴ� �޼ҵ�
	 *
	 * @param NewLocation - ���ο� ��� ��ġ
	 */
	void SetRelativeLocation(const FVector& NewLocation)
	{
		Transform.Location = NewLocation;
		TransformUpdate();
	}
	/**
	 * ���� ��� ��ġ�� ���� ���� ���Ͽ� �����ϴ� �޼ҵ�
	 *
	 * @param DeltaLocation - ���� ��� ��ġ�� ���氪
	 */
	void AddRelativeLocation(const FVector& DeltaLocation)
	{
		Transform.Location += DeltaLocation;
		TransformUpdate();
	}
	/**
	 * �θ� �������� ������Ʈ�� ��� ȸ���� �����ϴ� �޼ҵ�
	 *
	 * @param NewRotation - ���ο� ��� ȸ�� ��
	 */
	void SetRelativeRotation(const FVector& NewRotation)
	{
		Transform.Rotation = NewRotation;
		TransformUpdate();
	}
	/**
	 * ���� ��� ȸ���� ��Ÿ ȸ�� ���� ���Ͽ� �����ϴ� �޼ҵ�
	 *
	 * @param DeltaRotation - ���� ��� ȸ���� ���氪
	 */
	void AddRelativeRotation(const FVector& DeltaRotation)
	{
		Transform.Rotation += DeltaRotation;
		TransformUpdate();
	}
	/**
	 * �θ� �������� ������Ʈ�� ��� ũ�⸦ �����ϴ� �޼ҵ�
	 *
	 * @param NewScale - ���ο� ��� ũ��
	 */
	void SetRelativeScale3D(const FVector& NewScale)
	{
		bAbsolute = true;
		Transform.Scale = NewScale;
		TransformUpdate();
	}
	/**
	 * ���� ��� ũ�⿡ ��Ÿ ũ�� ���� ���Ͽ� �����ϴ� �޼ҵ�
	 *
	 * @param DeltaScale - ���� ��� ũ���� ���氪
	 */
	void AddRelativeScale3D(const FVector& DeltaScale)
	{
		Transform.Scale += DeltaScale;
		TransformUpdate();
	}
	/**
	 * ���� �������� ������Ʈ�� ���� ��ġ�� �����ϴ� �޼ҵ�
	 *
	 * @param NewLocation - ���ο� ���� ��ġ
	 */
	void SetWorldLocation(const FVector& NewLocation)
	{
		bAbsolute = true;
		Transform.Location = NewLocation;
		TransformUpdate();
	}
	/**
	 * ���� ���� ��ġ�� ��Ÿ ��ġ ���� ���Ͽ� �����ϴ� �޼ҵ�
	 *
	 * @param DeltaLocation - ���� ���� ��ġ�� ���氪
	 */
	void AddWorldLocation(FVector DeltaLocation)
	{
		Transform.Location += DeltaLocation;
		TransformUpdate();
	}
	/**
	 * ���� �������� ������Ʈ�� ���� ȸ���� �����ϴ� �޼ҵ�
	 *
	 * @param NewRotation - ���ο� ���� ȸ�� ��
	 */
	void SetWorldRotation(const FVector& NewRotation)
	{
		Transform.Rotation = NewRotation;
		TransformUpdate();
	}
	/**
	 * ���� ���� ȸ���� ��Ÿ ȸ�� ���� ���Ͽ� �����ϴ� �޼ҵ�
	 *
	 * @param DeltaRotation - ���� ���� ȸ���� ���氪
	 */
	void AddWorldRotation(const FVector& DeltaRotation)
	{
		Transform.Rotation += DeltaRotation;
		TransformUpdate();
	}
	/**
	 * ���� �������� ������Ʈ�� ���� ũ�⸦ �����ϴ� �޼ҵ�
	 *
	 * @param NewScale - ���ο� ���� ũ��
	 */
	void SetWorldScale3D(const FVector& NewScale)
	{
		bAbsolute = true;
		Transform.Scale = NewScale;
		TransformUpdate();
	}
	/**
	 * ���� ���� ũ�⿡ ��Ÿ ũ�� ���� ���Ͽ� �����ϴ� �޼ҵ�
	 *
	 * @param DeltaScale - ���� ���� ũ���� ���氪
	 */
	void AddWorldScale3D(const FVector& DeltaScale)
	{
		Transform.Scale += DeltaScale;
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
	bool IsAbsolute() const
	{
		return bAbsolute;
	}

protected:
	ENGINE_API void BeginPlay() override;
	void ParentMatrixCheck();

	FTransform Transform;
	bool bAbsolute = false;
private:

	USceneComponent* Parent = nullptr;
	std::list<std::shared_ptr<USceneComponent>> Childs;
};


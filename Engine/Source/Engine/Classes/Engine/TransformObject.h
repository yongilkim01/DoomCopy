#pragma once

/**
 *	����
 */
class UTransformObject
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UTransformObject();
	ENGINE_API virtual ~UTransformObject();

	/** ��ü �� ���� ���� */
	UTransformObject(const UTransformObject& _Other) = delete;
	UTransformObject(UTransformObject&& _Other) noexcept = delete;
	UTransformObject& operator=(const UTransformObject& _Other) = delete;
	UTransformObject& operator=(UTransformObject&& _Other) noexcept = delete;

	ENGINE_API void TransformUpdate();
	ENGINE_API void SetupAttachment(std::shared_ptr<UTransformObject> ParentComponent);
	ENGINE_API void SetupAttachment(UTransformObject* ParentComponent);

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
		Transform.Scale = NewScale;
		Transform.Scale.W = 0.0f;
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
		bAbsolute = true;
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
		bAbsolute = true;
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
	FVector GetWorldScale3D()
	{
		return Transform.WorldScale;
	}

protected:
	void ParentMatrixCheck();

	FTransform Transform;
	bool bAbsolute = false;

	UTransformObject* Parent = nullptr;
	std::list<UTransformObject*> Childs;

private:

};


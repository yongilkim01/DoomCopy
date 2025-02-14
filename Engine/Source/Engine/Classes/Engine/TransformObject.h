#pragma once

class UCameraComponent;

/**
 *	설명
 */
class UTransformObject
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UTransformObject();
	ENGINE_API virtual ~UTransformObject();

	/** 객체 값 복사 방지 */
	UTransformObject(const UTransformObject& _Other) = delete;
	UTransformObject(UTransformObject&& _Other) noexcept = delete;
	UTransformObject& operator=(const UTransformObject& _Other) = delete;
	UTransformObject& operator=(UTransformObject&& _Other) noexcept = delete;

	ENGINE_API virtual void UpdateCameraTransform(UCameraComponent* CameraComponent);

	ENGINE_API void TransformUpdate();
	ENGINE_API void SetupAttachment(std::shared_ptr<UTransformObject> ParentComponent);
	ENGINE_API void SetupAttachment(UTransformObject* ParentComponent);

	/**
	 * 부모 기준으로 컴포넌트의 상대 위치를 반환하는 메소드
	 *
	 */
	FVector GetRelativeLocation()
	{
		return Transform.RelativeLocation;
	}
	/**
	 * 부모 기준으로 컴포넌트의 상대 위치를 설정하는 메소드
	 *
	 * @param NewLocation - 새로운 상대 위치
	 */
	void SetRelativeLocation(const FVector& NewLocation)
	{
		Transform.Location = NewLocation;
		TransformUpdate();
	}
	/**
	 * 현재 상대 위치에 벡터 값을 더하여 설정하는 메소드
	 *
	 * @param DeltaLocation - 더할 상대 위치의 변경값
	 */
	void AddRelativeLocation(const FVector& DeltaLocation)
	{
		Transform.Location += DeltaLocation;
		TransformUpdate();
	}
	/**
	 * 부모 기준으로 컴포넌트의 상대 회전을 설정하는 메소드
	 *
	 * @param NewRotation - 새로운 상대 회전 값
	 */
	void SetRelativeRotation(const FVector& NewRotation)
	{
		Transform.Rotation = NewRotation;
		TransformUpdate();
	}
	/**
	 * 현재 상대 회전에 델타 회전 값을 더하여 설정하는 메소드
	 *
	 * @param DeltaRotation - 더할 상대 회전의 변경값
	 */
	void AddRelativeRotation(const FVector& DeltaRotation)
	{
		Transform.Rotation += DeltaRotation;
		TransformUpdate();
	}
	/**
	 * 부모 기준으로 컴포넌트의 상대 크기를 설정하는 메소드
	 *
	 * @param NewScale - 새로운 상대 크기
	 */
	void SetRelativeScale3D(const FVector& NewScale)
	{
		Transform.Scale = NewScale;
		Transform.Scale.W = 0.0f;
		TransformUpdate();
	}
	/**
	 * 현재 상대 크기에 델타 크기 값을 더하여 설정하는 메소드
	 *
	 * @param DeltaScale - 더할 상대 크기의 변경값
	 */
	void AddRelativeScale3D(const FVector& DeltaScale)
	{
		Transform.Scale += DeltaScale;
		TransformUpdate();
	}
	/**
	 * 월드 기준으로 컴포넌트의 절대 위치를 반환하는 메소드
	 *
	 */
	FVector GetWorldLocation()
	{
		return Transform.WorldLocation;
	}
	/**
	 * 월드 기준으로 컴포넌트의 절대 위치를 설정하는 메소드
	 *
	 * @param NewLocation - 새로운 절대 위치
	 */
	void SetWorldLocation(const FVector& NewLocation)
	{
		bAbsolute = true;
		Transform.Location = NewLocation;
		TransformUpdate();
	}
	/**
	 * 현재 절대 위치에 델타 위치 값을 더하여 설정하는 메소드
	 *
	 * @param DeltaLocation - 더할 절대 위치의 변경값
	 */
	void AddWorldLocation(FVector DeltaLocation)
	{
		bAbsolute = true;
		Transform.Location += DeltaLocation;
		TransformUpdate();
	}
	/**
	 * 월드 기준으로 컴포넌트의 절대 회전을 설정하는 메소드
	 *
	 * @param NewRotation - 새로운 절대 회전 값
	 */
	void SetWorldRotation(const FVector& NewRotation)
	{
		Transform.Rotation = NewRotation;
		TransformUpdate();
	}
	/**
	 * 현재 절대 회전에 델타 회전 값을 더하여 설정하는 메소드
	 *
	 * @param DeltaRotation - 더할 절대 회전의 변경값
	 */
	void AddWorldRotation(const FVector& DeltaRotation)
	{
		bAbsolute = true;
		Transform.Rotation += DeltaRotation;
		TransformUpdate();
	}
	/**
	 * 월드 기준으로 컴포넌트의 절대 크기를 설정하는 메소드
	 *
	 * @param NewScale - 새로운 절대 크기
	 */
	void SetWorldScale3D(const FVector& NewScale)
	{
		bAbsolute = true;
		Transform.Scale = NewScale;
		TransformUpdate();
	}
	/**
	 * 현재 절대 크기에 델타 크기 값을 더하여 설정하는 메소드
	 *
	 * @param DeltaScale - 더할 절대 크기의 변경값
	 */
	void AddWorldScale3D(const FVector& DeltaScale)
	{
		Transform.Scale += DeltaScale;
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
	bool IsAbsolute() const
	{
		return bAbsolute;
	}
	FVector GetWorldScale3D()
	{
		return Transform.WorldScale;
	}
	UTransformObject* GetParentTransformObject()
	{
		return Parent;
	}
	void SetParentTransformObject(UTransformObject* Value)
	{
		Parent = Value;
	}
	FVector GetComponentRotation()
	{
		return Transform.WorldRotation;
	}
	FVector GetWorldForwardVector()
	{
		return Transform.GetWorldFoward();
	}

protected:
	void ParentMatrixCheck();

	FTransform Transform;
	bool bAbsolute = false;

	UTransformObject* Parent = nullptr;
	std::list<UTransformObject*> Childs;

private:

};


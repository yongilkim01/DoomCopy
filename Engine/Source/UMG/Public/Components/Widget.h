#pragma once

/**
 *	설명
 */
class UWidget
{
public:
	/** 생성자, 소멸자 */
	UWidget();
	~UWidget();

	/** 객체 값 복사 방지 */
	UWidget(const UWidget& Other) = delete;
	UWidget(UWidget&& Other) noexcept = delete;
	UWidget& operator=(const UWidget& Other) = delete;
	UWidget& operator=(UWidget&& Other) noexcept = delete;

	ENGINE_API void TransformUpdate();

	void SetRelativeLocation(const FVector& NewLocation)
	{
		Transform.Location = NewLocation;
		TransformUpdate();
	}
	void AddRelativeLocation(const FVector& DeltaLocation)
	{
		Transform.Location += DeltaLocation;
		TransformUpdate();
	}
	void SetRelativeRotation(const FVector& NewRotation)
	{
		Transform.Rotation = NewRotation;
		TransformUpdate();
	}
	void AddRelativeRotation(const FVector& DeltaRotation)
	{
		Transform.Rotation += DeltaRotation;
		TransformUpdate();
	}
	void SetRelativeScale3D(const FVector& NewScale)
	{
		Transform.Scale = NewScale;
		Transform.Scale.W = 0.0f;
		TransformUpdate();
	}
	void AddRelativeScale3D(const FVector& DeltaScale)
	{
		Transform.Scale += DeltaScale;
		TransformUpdate();
	}
	void SetWorldLocation(const FVector& NewLocation)
	{
		bAbsolute = true;
		Transform.Location = NewLocation;
		TransformUpdate();
	}
	void AddWorldLocation(FVector DeltaLocation)
	{
		bAbsolute = true;
		Transform.Location += DeltaLocation;
		TransformUpdate();
	}
	void SetWorldRotation(const FVector& NewRotation)
	{
		Transform.Rotation = NewRotation;
		TransformUpdate();
	}
	void AddWorldRotation(const FVector& DeltaRotation)
	{
		bAbsolute = true;
		Transform.Rotation += DeltaRotation;
		TransformUpdate();
	}
	void SetWorldScale3D(const FVector& NewScale)
	{
		bAbsolute = true;
		Transform.Scale = NewScale;
		TransformUpdate();
	}
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

protected:

private:
	FTransform Transform;
	bool bAbsolute = false;

	UWidget* ParentWidget = nullptr;
	std::list<std::shared_ptr<UWidget>> ChildWidgetList;
};


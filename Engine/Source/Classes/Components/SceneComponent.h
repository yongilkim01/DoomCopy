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
	ENGINE_API USceneComponent();
	ENGINE_API ~USceneComponent();

	/** 객체 값 복사 방지 */
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	ENGINE_API void TransformUpdate();
	ENGINE_API void SetupAttachment(std::shared_ptr<USceneComponent> ParentComponent);
	void SetupAttachment(USceneComponent* ParentComponent);

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
		bAbsolute = true;
		Transform.Scale = NewScale;
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

protected:
	ENGINE_API void BeginPlay() override;
	void ParentMatrixCheck();

	FTransform Transform;
	bool bAbsolute = false;
private:

	USceneComponent* Parent = nullptr;
	std::list<std::shared_ptr<USceneComponent>> Childs;
};


#pragma once
#include "GameFramework/Actor.h"

class UCameraComponent;

/**
 *	카메라 컴포넌트 클래스
 */
class ACameraActor : public AActor
{
public:
	/** 생성자, 소멸자 */
	ACameraActor();
	~ACameraActor();

	/** 객체 값 복사 방지 */
	ACameraActor(const ACameraActor& Other) = delete;
	ACameraActor(ACameraActor&& Other) noexcept = delete;
	ACameraActor& operator=(const ACameraActor& Other) = delete;
	ACameraActor& operator=(ACameraActor&& Other) noexcept = delete;

	ENGINE_API FVector ScreenMouseLocationToWorldLocation();
	// 항상 중심을 0,0 으로 보는 마우스 포스 얻는법
	ENGINE_API FVector ScreenMouseLocationToWorldLocationWithOutLocation();

	/** 액터 상속 메소드 */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** 겟, 셋 메소드 */
	ENGINE_API std::shared_ptr<UCameraComponent> GetCameraComponent()
	{
		return CameraComponent;
	}

protected:

private:
	std::shared_ptr<UCameraComponent> CameraComponent = nullptr;
};

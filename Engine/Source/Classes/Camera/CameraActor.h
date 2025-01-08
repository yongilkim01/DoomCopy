#pragma once
#include "GameFramework/Actor.h"
#include "EngineEnums.h"

class UCameraComponent;

/**
 *	ī�޶� ������Ʈ Ŭ����
 */
class ACameraActor : public AActor
{
public:
	/** ������, �Ҹ��� */
	ACameraActor();
	~ACameraActor();

	/** ��ü �� ���� ���� */
	ACameraActor(const ACameraActor& Other) = delete;
	ACameraActor(ACameraActor&& Other) noexcept = delete;
	ACameraActor& operator=(const ACameraActor& Other) = delete;
	ACameraActor& operator=(ACameraActor&& Other) noexcept = delete;

	ENGINE_API FVector ScreenMouseLocationToWorldLocation();
	// �׻� �߽��� 0,0 ���� ���� ���콺 ���� ��¹�
	ENGINE_API FVector ScreenMouseLocationToWorldLocationWithOutLocation();
	ENGINE_API FVector GetMouseLocation();
	
	ENGINE_API void OnFreeCamera();
	ENGINE_API void OffFreeCamera();
	ENGINE_API void SwitchFreeCamera();

	/** ���� ��� �޼ҵ� */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** ��, �� �޼ҵ� */
	ENGINE_API std::shared_ptr<UCameraComponent> GetCameraComponent()
	{
		return CameraComponent;
	}
	ENGINE_API inline bool IsFreeCamera()
	{
		return bFreeCameraValue;
	}
	// #ifdef _DEBUG
	ENGINE_API void SetFreeCameraSpeed(float NewSpeed)
	{
		FreeSpeed = NewSpeed;
	}
	// #endif

protected:

private:
	ENGINE_API void CheckCameraCheck();

	std::shared_ptr<UCameraComponent> CameraComponent = nullptr;

	EProjectionType PrevProjectionType = EProjectionType::Orthographic;

	FTransform PrevTrans;
	FVector ScreenPos;
	FVector PrevScreenPos;

	float FreeSpeed = 500.0f;
	float RotSpeed = 720.0f;
	bool bFreeCameraValue = false;
};

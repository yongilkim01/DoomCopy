#pragma once
#include "GameFramework/Actor.h"

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

	/** ���� ��� �޼ҵ� */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** ��, �� �޼ҵ� */
	std::shared_ptr<UCameraComponent> GetCameraComponent()
	{
		return CameraComponent;
	}

protected:

private:
	std::shared_ptr<UCameraComponent> CameraComponent = nullptr;
};

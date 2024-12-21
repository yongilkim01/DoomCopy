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

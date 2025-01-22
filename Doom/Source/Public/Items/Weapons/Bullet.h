#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class ACharacter;

/**
 *	����
 */
class ABullet : public AActor
{
public:
	/** ������, �Ҹ��� */
	ABullet();
	~ABullet();

	/** ��ü �� ���� ���� */
	ABullet(const ABullet& Other) = delete;
	ABullet(ABullet&& Other) noexcept = delete;
	ABullet& operator=(const ABullet& Other) = delete;
	ABullet& operator=(ABullet&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	/** ������ */
	std::weak_ptr<ACharacter> Owner;
};
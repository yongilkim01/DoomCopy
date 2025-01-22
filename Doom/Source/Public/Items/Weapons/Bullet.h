#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class ACharacter;

/**
 *	설명
 */
class ABullet : public AActor
{
public:
	/** 생성자, 소멸자 */
	ABullet();
	~ABullet();

	/** 객체 값 복사 방지 */
	ABullet(const ABullet& Other) = delete;
	ABullet(ABullet&& Other) noexcept = delete;
	ABullet& operator=(const ABullet& Other) = delete;
	ABullet& operator=(ABullet&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	/** 소유주 */
	std::weak_ptr<ACharacter> Owner;
};
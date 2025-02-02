#pragma once
#include "Public/Items/DropItems/DropItem.h"

/**
 *	설명
 */
class ADropShotgun : public ADropItem
{
public:
	/** 생성자, 소멸자 */
	ADropShotgun();
	~ADropShotgun();

	/** 객체 값 복사 방지 */
	ADropShotgun(const ADropShotgun& _Other) = delete;
	ADropShotgun(ADropShotgun&& _Other) noexcept = delete;
	ADropShotgun& operator=(const ADropShotgun& _Other) = delete;
	ADropShotgun& operator=(ADropShotgun&& _Other) noexcept = delete;

	/** 액터 상속 메소드 */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** ADropItem 상속 메소드 */
	virtual void EntryDrop() override;
	virtual void EntryPickup() override;
	virtual void Drop(float DeltaTime) override;
	virtual void Pickup(float DeltaTime) override;

protected:

private:

};


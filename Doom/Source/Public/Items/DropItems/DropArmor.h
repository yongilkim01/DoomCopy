#pragma once
#include "Public/Items/DropItems/DropItem.h"

/**
 *	설명
 */
class ADropArmor : public ADropItem
{
public:
	/** 생성자, 소멸자 */
	ADropArmor();
	~ADropArmor();

	/** 객체 값 복사 방지 */
	ADropArmor(const ADropArmor& _Other) = delete;
	ADropArmor(ADropArmor&& _Other) noexcept = delete;
	ADropArmor& operator=(const ADropArmor& _Other) = delete;
	ADropArmor& operator=(ADropArmor&& _Other) noexcept = delete;

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


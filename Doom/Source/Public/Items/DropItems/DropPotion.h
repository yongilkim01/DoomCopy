#pragma once
#include "Public/Items/DropItems/DropItem.h"

/**
 *	설명
 */
class ADropPotion : public ADropItem
{
public:
	/** 생성자, 소멸자 */
	ADropPotion();
	~ADropPotion();

	/** 객체 값 복사 방지 */
	ADropPotion(const ADropPotion& _Other) = delete;
	ADropPotion(ADropPotion&& _Other) noexcept = delete;
	ADropPotion& operator=(const ADropPotion& _Other) = delete;
	ADropPotion& operator=(ADropPotion&& _Other) noexcept = delete;

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


#pragma once
#include "Public/Items/DropItems/DropItem.h"

/**
 *	����
 */
class ADropPotion : public ADropItem
{
public:
	/** ������, �Ҹ��� */
	ADropPotion();
	~ADropPotion();

	/** ��ü �� ���� ���� */
	ADropPotion(const ADropPotion& _Other) = delete;
	ADropPotion(ADropPotion&& _Other) noexcept = delete;
	ADropPotion& operator=(const ADropPotion& _Other) = delete;
	ADropPotion& operator=(ADropPotion&& _Other) noexcept = delete;

	/** ���� ��� �޼ҵ� */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** ADropItem ��� �޼ҵ� */
	virtual void EntryDrop() override;
	virtual void EntryPickup() override;
	virtual void Drop(float DeltaTime) override;
	virtual void Pickup(float DeltaTime) override;

protected:

private:

};


#pragma once
#include "Public/Items/DropItems/DropItem.h"

/**
 *	����
 */
class ADropArmor : public ADropItem
{
public:
	/** ������, �Ҹ��� */
	ADropArmor();
	~ADropArmor();

	/** ��ü �� ���� ���� */
	ADropArmor(const ADropArmor& _Other) = delete;
	ADropArmor(ADropArmor&& _Other) noexcept = delete;
	ADropArmor& operator=(const ADropArmor& _Other) = delete;
	ADropArmor& operator=(ADropArmor&& _Other) noexcept = delete;

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


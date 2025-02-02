#pragma once
#include "Public/Items/DropItems/DropItem.h"

/**
 *	����
 */
class ADropShotgun : public ADropItem
{
public:
	/** ������, �Ҹ��� */
	ADropShotgun();
	~ADropShotgun();

	/** ��ü �� ���� ���� */
	ADropShotgun(const ADropShotgun& _Other) = delete;
	ADropShotgun(ADropShotgun&& _Other) noexcept = delete;
	ADropShotgun& operator=(const ADropShotgun& _Other) = delete;
	ADropShotgun& operator=(ADropShotgun&& _Other) noexcept = delete;

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


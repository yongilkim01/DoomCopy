#pragma once
#include <Engine/Classes/GameFramework/Actor.h>
#include "Public/Global/DoomEnums.h"

class ADoomGuyCharacter;

/**
 *	����
 */
class ADropItem : public AActor
{
public:
	/** ������, �Ҹ��� */
	ADropItem();
	~ADropItem();

	/** ��ü �� ���� ���� */
	ADropItem(const ADropItem& _Other) = delete;
	ADropItem(ADropItem&& _Other) noexcept = delete;
	ADropItem& operator=(const ADropItem& _Other) = delete;
	ADropItem& operator=(ADropItem&& _Other) noexcept = delete;

	/** ���� ��� �޼ҵ� */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** ADropItem ��� �޼ҵ� */
	virtual void EntryDrop();
	virtual void EntryPickup();
	virtual void Drop(float DeltaTime);
	virtual void Pickup(float DeltaTime);

	/** ADropItem ���� �޼ҵ� */
	void ChangeState(EDropItemState State);

	/** ��, �� �޼ҵ� */
	void SetDropItemType(EDropItemType Type)
	{
		CurDropItemType = Type;
	}
	EDropItemType GetDropItemType()
	{
		return CurDropItemType;
	}
	void SetDropItemState(EDropItemState State)
	{
		CurDropItemState = State;
	}
	EDropItemState GetDropItemState()
	{
		return CurDropItemState;
	}
	ADoomGuyCharacter* GetDoomGuy()
	{
		return DoomGuyCharacter;
	}

protected:

private:
	EDropItemType CurDropItemType = EDropItemType::NONE;
	EDropItemState CurDropItemState = EDropItemState::NONE;
	ADoomGuyCharacter* DoomGuyCharacter = nullptr;
};


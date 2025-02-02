#pragma once
#include <Engine/Classes/GameFramework/Actor.h>
#include "Public/Global/DoomEnums.h"

class ADoomGuyCharacter;

/**
 *	설명
 */
class ADropItem : public AActor
{
public:
	/** 생성자, 소멸자 */
	ADropItem();
	~ADropItem();

	/** 객체 값 복사 방지 */
	ADropItem(const ADropItem& _Other) = delete;
	ADropItem(ADropItem&& _Other) noexcept = delete;
	ADropItem& operator=(const ADropItem& _Other) = delete;
	ADropItem& operator=(ADropItem&& _Other) noexcept = delete;

	/** 액터 상속 메소드 */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** ADropItem 상속 메소드 */
	virtual void EntryDrop();
	virtual void EntryPickup();
	virtual void Drop(float DeltaTime);
	virtual void Pickup(float DeltaTime);

	/** ADropItem 공통 메소드 */
	void ChangeState(EDropItemState State);

	/** 겟, 셋 메소드 */
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


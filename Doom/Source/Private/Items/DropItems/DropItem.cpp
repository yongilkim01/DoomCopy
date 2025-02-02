#include "Doom.h"
#include "Public/Items/DropItems/DropItem.h"

ADropItem::ADropItem()
{
}

ADropItem::~ADropItem()
{
}

void ADropItem::BeginPlay()
{
	AActor::BeginPlay();
}

void ADropItem::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
	
	switch (CurDropItemState)
	{
	case EDropItemState::DROP:
		Drop(DeltaTime);
		break;
	case EDropItemState::PICKUP:
		Pickup(DeltaTime);
		break;
	default:
		break;
	}
}

void ADropItem::EntryDrop()
{
}

void ADropItem::EntryPickup()
{
}

void ADropItem::Drop(float DeltaTime)
{
}

void ADropItem::Pickup(float DeltaTime)
{
}

void ADropItem::ChangeState(EDropItemState State)
{
	CurDropItemState = State;

	switch (CurDropItemState)
	{
	case EDropItemState::DROP:
		EntryDrop();
		break;
	case EDropItemState::PICKUP:
		EntryPickup();
		break;
	default:
		break;
	}
}

#include "Doom.h"
#include "Public/Items/DropItems/DropArmor.h"

ADropArmor::ADropArmor()
{
}

ADropArmor::~ADropArmor()
{
}

void ADropArmor::BeginPlay()
{
	ADropItem::BeginPlay();

}

void ADropArmor::Tick(float DeltaTime)
{
	ADropItem::Tick(DeltaTime);
}

void ADropArmor::EntryDrop()
{
}

void ADropArmor::EntryPickup()
{
}

void ADropArmor::Drop(float DeltaTime)
{
}

void ADropArmor::Pickup(float DeltaTime)
{
}

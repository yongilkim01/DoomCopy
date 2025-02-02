#include "Doom.h"
#include "Public/Items/DropItems/DropShotgun.h"

ADropShotgun::ADropShotgun()
{
}

ADropShotgun::~ADropShotgun()
{
}

void ADropShotgun::BeginPlay()
{
	ADropItem::BeginPlay();
}

void ADropShotgun::Tick(float DeltaTime)
{
	ADropItem::Tick(DeltaTime);

}

void ADropShotgun::EntryDrop()
{
}

void ADropShotgun::EntryPickup()
{
}

void ADropShotgun::Drop(float DeltaTime)
{
}

void ADropShotgun::Pickup(float DeltaTime)
{
}

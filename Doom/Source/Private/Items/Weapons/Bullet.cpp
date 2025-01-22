#include "pch.h"
#include "Public/Items/Weapons/Bullet.h"

#include <Engine/Classes/Components/SceneComponent.h>

ABullet::ABullet()
{
	std::shared_ptr<USceneComponent> SceneComponent = CreateDefaultSubObject<USceneComponent>();
	RootComponent = SceneComponent;
}

ABullet::~ABullet()
{
}

void ABullet::BeginPlay()
{
}

void ABullet::Tick(float DeltaTime)
{
}

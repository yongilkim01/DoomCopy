#include "pch.h"
#include "ActorComponent.h"
#include "GameFramework/Actor.h"

UActorComponent::UActorComponent()
{
}

UActorComponent::~UActorComponent()
{
}

ULevel* UActorComponent::GetWorld()
{
    return GetOwner()->GetWorld();
}

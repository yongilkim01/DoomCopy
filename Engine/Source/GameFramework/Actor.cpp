#include "pch.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/ActorComponent.h"

AActor::AActor()
{
}

AActor::~AActor()
{
    RootComponent = nullptr;
}

void AActor::BeginPlay()
{
    if (nullptr != RootComponent)
    {
        RootComponent->BeginPlay();
    }

    for (std::shared_ptr<UActorComponent>& ActorComponent : ActorComponentList)
    {
        ActorComponent->BeginPlay();
    }
}

void AActor::Tick(float DeltaTime)
{
    if (nullptr != RootComponent)
    {
        RootComponent->ComponentTick(DeltaTime);
    }

    for (std::shared_ptr<class UActorComponent> ActorComponent : ActorComponentList)
    {
        ActorComponent->ComponentTick(DeltaTime);
    }
}
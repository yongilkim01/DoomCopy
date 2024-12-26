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

}
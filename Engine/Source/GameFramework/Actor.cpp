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

void AActor::AttachToActor(AActor* Parent)
{
    if (nullptr == RootComponent)
    {
        MSGASSERT("�ڽ��� RootComponent�� nullptr�Դϴ�.");
        return;
    }
    if (nullptr == Parent->RootComponent)
    {
        MSGASSERT("�θ��� RootComponent�� nullptr�Դϴ�.");
        return;
    }
    RootComponent->SetupAttachment(Parent->RootComponent);
}

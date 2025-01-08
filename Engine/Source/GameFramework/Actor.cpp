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
    for (std::shared_ptr<AActor>& Actor : ChildList)
    {
        RootComponent->ComponentTick(DeltaTime);

        Actor->Tick(DeltaTime);
    }

    if (nullptr == Parent)
    {
        if (nullptr != RootComponent)
        {
            RootComponent->ComponentTick(DeltaTime);
        }
    }

    for (std::shared_ptr<class UActorComponent> ActorComponent : ActorComponentList)
    {
        if (false == ActorComponent->IsActive())
        {
            continue;
        }

        ActorComponent->ComponentTick(DeltaTime);
    }
}

void AActor::AttachToActor(AActor* NewParent)
{
    if (nullptr == RootComponent)
    {
        MSGASSERT("�ڽ��� RootComponent�� nullptr�Դϴ�.");
        return;
    }
    if (nullptr == NewParent->RootComponent)
    {
        MSGASSERT("�θ��� RootComponent�� nullptr�Դϴ�.");
        return;
    }

    Parent = NewParent;

    NewParent->ChildList.push_back(GetThis<AActor>());

    RootComponent->SetupAttachment(NewParent->RootComponent);
}

FVector AActor::GetActorUpVector()
{
    if (nullptr == RootComponent)
    {
        return FVector(0.0f, 0.0f, 0.0f, 1.0f);
    }

    return RootComponent->GetComponentTransformRef().World.GetUp();
}

FVector AActor::GetActorRightVector()
{
    if (nullptr == RootComponent)
    {
        return FVector(0.0f, 0.0f, 0.0f, 1.0f);
    }
    return RootComponent->GetComponentTransformRef().World.GetRight();
}

FVector AActor::GetActorForwardVector()
{
    if (nullptr == RootComponent)
    {
        return FVector(0.0f, 0.0f, 0.0f, 1.0f);
    }
    return RootComponent->GetComponentTransformRef().World.GetFoward();
}

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
        MSGASSERT("자식의 RootComponent가 nullptr입니다.");
        return;
    }
    if (nullptr == Parent->RootComponent)
    {
        MSGASSERT("부모의 RootComponent가 nullptr입니다.");
        return;
    }
    RootComponent->SetupAttachment(Parent->RootComponent);
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

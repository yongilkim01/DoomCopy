#include "Engine.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Components/ActorComponent.h"

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
        RootComponent->TickComponent(DeltaTime);

        Actor->Tick(DeltaTime);
    }

    if (nullptr == Parent)
    {
        if (nullptr != RootComponent)
        {
            RootComponent->TickComponent(DeltaTime);
        }
    }

    for (std::shared_ptr<class UActorComponent> ActorComponent : ActorComponentList)
    {
        if (false == ActorComponent->IsActive())
        {
            continue;
        }

        ActorComponent->TickComponent(DeltaTime);
    }
}

void AActor::AttachToActor(AActor* NewParent)
{
    if (nullptr == RootComponent)
    {
        MSGASSERT("자식의 RootComponent가 nullptr입니다.");
        return;
    }
    if (nullptr == NewParent->RootComponent)
    {
        MSGASSERT("부모의 RootComponent가 nullptr입니다.");
        return;
    }

    Parent = NewParent;

    NewParent->ChildList.push_back(GetThis<AActor>());

    RootComponent->SetupAttachment(NewParent->RootComponent);
}

UGameInstance* AActor::GetGameInstance()
{
    return GEngine->GetGameInstance();
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

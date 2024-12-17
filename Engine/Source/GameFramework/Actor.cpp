#include "pch.h"
#include "GameFramework/Actor.h"

AActor::AActor()
{
}

AActor::~AActor()
{
}

void AActor::BeginPlay()
{
    if (nullptr != RootComponent)
    {
        RootComponent->BeginPlay();
    }
}

void AActor::Tick(float DeltaTime)
{

}
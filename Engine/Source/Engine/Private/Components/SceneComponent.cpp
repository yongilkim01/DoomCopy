#include "pch.h"
#include "Engine/Classes/Components/SceneComponent.h"

USceneComponent::USceneComponent()
{
}

USceneComponent::~USceneComponent()
{
}

void USceneComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	for (UTransformObject* Child : Childs)
	{
		USceneComponent* SceneComponentChild = dynamic_cast<USceneComponent*>(Child);
		SceneComponentChild->BeginPlay();
	}
}

void USceneComponent::ComponentTick(float DeltaTime)
{
	UActorComponent::ComponentTick(DeltaTime);

	for (UTransformObject* Child : Childs)
	{
		USceneComponent* SceneComponentChild = dynamic_cast<USceneComponent*>(Child);

		if (false == SceneComponentChild->IsActive())
		{
			continue;
		}

		SceneComponentChild->ComponentTick(DeltaTime);
	}
}
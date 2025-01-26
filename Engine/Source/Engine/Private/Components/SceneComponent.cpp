#include "Engine.h"
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

void USceneComponent::TickComponent(float DeltaTime)
{
	UActorComponent::TickComponent(DeltaTime);

	for (UTransformObject* Child : Childs)
	{
		USceneComponent* SceneComponentChild = dynamic_cast<USceneComponent*>(Child);

		if (false == SceneComponentChild->IsActive())
		{
			continue;
		}

		SceneComponentChild->TickComponent(DeltaTime);
	}
}
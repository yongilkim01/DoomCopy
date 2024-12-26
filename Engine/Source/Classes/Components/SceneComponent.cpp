#include "pch.h"
#include "SceneComponent.h"

USceneComponent::USceneComponent()
{
}

USceneComponent::~USceneComponent()
{
}

void USceneComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	for (std::shared_ptr<USceneComponent> ChildComponent : Childs)
	{
		ChildComponent->BeginPlay();
	}
}

void USceneComponent::TransformUpdate()
{
	ParentMatrixCheck();

	Transform.TransformUpdate();

	for (std::shared_ptr<USceneComponent> ChildComponent : Childs)
	{
		ChildComponent->TransformUpdate();
	}
}

void USceneComponent::SetupAttachment(std::shared_ptr<USceneComponent> ParentComponent)
{
	SetupAttachment(ParentComponent.get());
}

void USceneComponent::SetupAttachment(USceneComponent* ParentComponent)
{
	Parent = ParentComponent;
	Parent->Childs.push_back(GetThis<USceneComponent>());

	TransformUpdate();
}

void USceneComponent::ParentMatrixCheck()
{
	if (nullptr != Parent)
	{
		Transform.ParentMat = Parent->Transform.World;
	}
}


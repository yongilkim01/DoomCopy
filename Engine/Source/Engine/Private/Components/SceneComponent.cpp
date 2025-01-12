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

	for (std::shared_ptr<USceneComponent> ChildComponent : Childs)
	{
		ChildComponent->BeginPlay();
	}
}

void USceneComponent::ComponentTick(float DeltaTime)
{
	UActorComponent::ComponentTick(DeltaTime);

	for (std::shared_ptr<USceneComponent> Child : Childs)
	{
		if (false == Child->IsActive())
		{
			continue;
		}

		Child->ComponentTick(DeltaTime);
	}
}

void USceneComponent::TransformUpdate()
{
	ParentMatrixCheck();

	Transform.TransformUpdate(bAbsolute);

	for (std::shared_ptr<USceneComponent> ChildComponent : Childs)
	{
		ChildComponent->TransformUpdate();
	}

	bAbsolute = false;
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


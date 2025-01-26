#include "Engine.h"
#include "Engine/Classes/Components/ActorComponent.h"
#include "Engine/Classes/GameFramework/Actor.h"

UActorComponent::UActorComponent()
{
}

UActorComponent::~UActorComponent()
{
}

ULevel* UActorComponent::GetWorld()
{
    return GetOwner()->GetWorld();
}

bool UActorComponent::IsActive()
{
	if (nullptr == GetOwner())
	{
		MSGASSERT("부모가 존재하지 않는 컴포넌트가 존재합니다");
		return false;
	}

	return UObject::IsActive() && GetOwner()->IsActive();
}

bool UActorComponent::IsDestroy()
{
	return UObject::IsDestroy() || GetOwner()->IsDestroy();
}

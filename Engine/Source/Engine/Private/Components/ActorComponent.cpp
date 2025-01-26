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
		MSGASSERT("�θ� �������� �ʴ� ������Ʈ�� �����մϴ�");
		return false;
	}

	return UObject::IsActive() && GetOwner()->IsActive();
}

bool UActorComponent::IsDestroy()
{
	return UObject::IsDestroy() || GetOwner()->IsDestroy();
}

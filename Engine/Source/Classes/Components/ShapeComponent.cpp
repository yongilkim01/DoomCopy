#include "pch.h"
#include "ShapeComponent.h"

#include "Core/Containers/EngineString.h"
#include "Classes/Camera/CameraComponent.h"

UShapeComponent::UShapeComponent()
{
}

UShapeComponent::~UShapeComponent()
{
}

void UShapeComponent::BeginPlay()
{

}

void UShapeComponent::SetCollisionProfileName(std::string_view NewProfileName)
{
	if (NewProfileName == GetCollisionProfileName())
	{
		return;
	}
	std::string PrevProfileName = GetCollisionProfileName();
	ProfileName = NewProfileName;
	
	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<UShapeComponent> pComponent = GetThis<UShapeComponent>();

	//Level->ChangeCo

}


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
	USceneComponent::BeginPlay();
}

bool UShapeComponent::CollisionCheck(std::string_view OtherName, std::vector<UShapeComponent*>& ShapeVector)
{
	std::map<std::string_view, std::list<std::shared_ptr<UShapeComponent>>>& Collision = GetWorld()->ShapeCompMap;

	if (false == Collision.contains(OtherName))
	{
		MSGASSERT("존재하지 않는 그룹과 충돌할수 없습니다" + std::string(OtherName));
		return false;
	}

	std::list<std::shared_ptr<UShapeComponent>>& Group = Collision[OtherName];

	for (std::shared_ptr<UShapeComponent>& OtherCol : Group)
	{
		if (false == OtherCol->IsActive())
		{
			continue;
		}

		if (true == FTransform::Collision(CollisionType, Transform, OtherCol->CollisionType, OtherCol->Transform))
		{
			ShapeVector.push_back(OtherCol.get());
		}
	}

	return 0 != ShapeVector.size();
}

void UShapeComponent::SetRadius(float Radius)
{
	FVector Scale = GetWorldScale3D();
	Scale.X = Radius * 2.0f;
	SetWorldScale3D(Scale);
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


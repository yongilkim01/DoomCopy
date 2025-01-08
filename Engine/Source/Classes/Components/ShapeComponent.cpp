#include "pch.h"
#include "ShapeComponent.h"

#include "Core/Containers/EngineString.h"
#include "Classes/Camera/CameraComponent.h"

UShapeComponent::UShapeComponent()
{
}

UShapeComponent::~UShapeComponent()
{
	for (UShapeComponent* Other : CollisionCheckSet)
	{
		Other->CollisionCheckSet.erase(this);
	}
}

void UShapeComponent::BeginPlay()
{
	USceneComponent::BeginPlay();
}


void UShapeComponent::SetCollisionProfileName(std::string_view NewProfileName)
{
	if (NewProfileName == GetCollisionProfileName())
	{
		return;
	}
	std::string UpperName = UEngineString::ToUpper(NewProfileName);

	std::string PrevProfileName = GetCollisionProfileName();

	ProfileName = UpperName;

	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<UShapeComponent> pComponent = GetThis<UShapeComponent>();

	Level->ChangeCollisionProfileName(ProfileName, PrevProfileName, pComponent);

}

bool UShapeComponent::CollisionCheck(std::string_view OtherName, std::vector<UShapeComponent*>& ShapeVector)
{
	std::string UpperName = UEngineString::ToUpper(OtherName);

	std::map<std::string, std::list<std::shared_ptr<UShapeComponent>>>& Collision = GetWorld()->ShapeCompMap;

	if (false == Collision.contains(UpperName))
	{
		MSGASSERT("�������� �ʴ� �׷�� �浹�Ҽ� �����ϴ�" + std::string(UpperName));
		return false;
	}

	std::list<std::shared_ptr<UShapeComponent>>& Group = Collision[UpperName];

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

void UShapeComponent::CollisionEventCheck(std::shared_ptr<UShapeComponent> OtherComp)
{
	if (true == FTransform::Collision(CollisionType, Transform, OtherComp->CollisionType, OtherComp->Transform))
	{
		if (false == CollisionCheckSet.contains(OtherComp.get()))
		{
			CollisionCheckSet.insert(OtherComp.get());
			// ���ʹ� �÷��̾ ����մϴ�.
			OtherComp->CollisionCheckSet.insert(this);
			if (nullptr != Enter)
			{
				Enter(this, OtherComp.get());
			}
		}
		else
		{
			// �浹�� �ߴµ� ���� ���� �ε�ģ���� �ִ�.
			if (nullptr != Stay)
			{
				Stay(this, OtherComp.get());
			}
		}
	}
	else
	{
		if (true == CollisionCheckSet.contains(OtherComp.get()))
		{
			if (nullptr != End)
			{
				End(this, OtherComp.get());
			}
			// ������ �������� ��뼺
			CollisionCheckSet.erase(OtherComp.get());
			OtherComp->CollisionCheckSet.erase(this);
		}
	}
}

void UShapeComponent::SetCollisionEnter(std::function<void(UShapeComponent*, UShapeComponent*)> EventFunc)
{
	if ("NONE" == GetCollisionProfileName())
	{
		MSGASSERT("���� �浹 �׷��� �������� ���� �浹ü�� �̺�Ʈ ����Ϸ��� �߽��ϴ�");
		return;
	}

	if (true == IsEvent())
	{
		Enter = EventFunc;
		return;
	}

	Enter = EventFunc;

	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<UShapeComponent> ThisPtr = GetThis<UShapeComponent>();
	Level->CheckShapeCompMap[GetCollisionProfileName()].push_back(ThisPtr);
}

void UShapeComponent::SetCollisionStay(std::function<void(UShapeComponent*, UShapeComponent*)> EventFunc)
{
	if ("NONE" == GetCollisionProfileName())
	{
		MSGASSERT("���� �浹 �׷��� �������� ���� �浹ü�� �̺�Ʈ ����Ϸ��� �߽��ϴ�");
		return;
	}
	if (true == IsEvent())
	{
		Stay = EventFunc;
		return;
	}

	Stay = EventFunc;
	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<UShapeComponent> ThisPtr = GetThis<UShapeComponent>();
	Level->CheckShapeCompMap[GetCollisionProfileName()].push_back(ThisPtr);
}

void UShapeComponent::SetCollisionEnd(std::function<void(UShapeComponent*, UShapeComponent*)> EventFunc)
{
	if ("NONE" == GetCollisionProfileName())
	{
		MSGASSERT("���� �浹 �׷��� �������� ���� �浹ü�� �̺�Ʈ ����Ϸ��� �߽��ϴ�");
		return;
	}
	if (true == IsEvent())
	{
		Stay = EventFunc;
		return;
	}

	Stay = EventFunc;
	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<UShapeComponent> ThisPtr = GetThis<UShapeComponent>();
	Level->CheckShapeCompMap[GetCollisionProfileName()].push_back(ThisPtr);
}


void UShapeComponent::SetRadius(float Radius)
{
	FVector Scale = GetWorldScale3D();
	Scale.X = Radius * 2.0f;
	SetWorldScale3D(Scale);
}

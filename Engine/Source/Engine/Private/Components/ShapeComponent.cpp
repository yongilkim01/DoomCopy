#include "Engine.h"
#include "Engine/Classes/Components/ShapeComponent.h"

#include "Core/Public/Containers/EngineString.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Engine/RenderAsset.h"

UShapeComponent::UShapeComponent()
{
}

UShapeComponent::~UShapeComponent()
{

}

void UShapeComponent::BeginPlay()
{
	UPrimitiveComponent::BeginPlay();
}

void UShapeComponent::TickComponent(float DeltaTime)
{
	UPrimitiveComponent::TickComponent(DeltaTime);
}

void UShapeComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UPrimitiveComponent::Render(CameraComponent, DeltaTime);
}


void UShapeComponent::DebugRender(UCameraComponent* CameraComponent, float DeltaTime)
{
	URenderAsset RenderUnit;

	FTransform& CameraTransform = CameraComponent->GetComponentTransformRef();
	FTransform& ComponentTransform = GetComponentTransformRef();

	ComponentTransform.View = CameraTransform.View;
	ComponentTransform.Projection = CameraTransform.Projection;
	ComponentTransform.WVP = ComponentTransform.World * ComponentTransform.View * ComponentTransform.Projection;

	RenderUnit.SetMesh("Sphere");
	RenderUnit.SetMaterial("CollisionDebugMaterial");

	RenderUnit.ConstantBufferLinkData("FTransform", GetComponentTransformRef());
	FVector Color = { 0.0f, 1.0f, 0.0f };
	RenderUnit.ConstantBufferLinkData("OutColor", Color);

	RenderUnit.Render(CameraComponent, DeltaTime);
}

bool UShapeComponent::CollisionCheck(std::string_view OtherName, FVector NextLocation, std::vector<UShapeComponent*>& _Vector)
{
	std::string UpperName = UEngineString::ToUpper(OtherName);
	std::map<std::string, std::list<std::shared_ptr<UShapeComponent>>>& Collision = GetWorld()->ShapeCompMap;

	if (false == Collision.contains(UpperName))
	{
		MSGASSERT("�������� �ʴ� �׷�� �浹�Ҽ� �����ϴ� " + std::string(UpperName));
		return false;
	}

	FTransform NextTransform = Transform;
	NextTransform.Location += NextLocation;
	NextTransform.TransformUpdate();
	std::list<std::shared_ptr<UShapeComponent>>& Group = Collision[UpperName];

	for (std::shared_ptr<UShapeComponent>& OtherCol : Group)
	{
		if (false == OtherCol->IsActive())
		{
			continue;
		}
		if (true == FTransform::Collision(CollisionType, NextTransform, OtherCol->CollisionType, OtherCol->Transform))
		{
			_Vector.push_back(OtherCol.get());
		}
	}

	return 0 != _Vector.size();
}


void UShapeComponent::SetCollisionProfileName(std::string_view NewProfileName)
{
	// ���� ������ Collision Profile �̸��� �����ϴٸ� �۾��� �������� ����
	if (NewProfileName == GetCollisionProfileName())
	{
		return;
	}

	// ���޹��� Collision Profile �̸��� �빮�ڷ� ��ȯ
	std::string UpperName = UEngineString::ToUpper(NewProfileName);

	// ���� Collision Profile �̸��� ���� (���� ���� �̸�)
	std::string PrevProfileName = GetCollisionProfileName();

	// ProfileName ��� ������ ���ο� �̸��� ����
	ProfileName = UpperName;

	// ������Ʈ�� �Ҽӵ� ���� ������ ������
	ULevel* Level = GetOwner()->GetWorld();

	// ���� ��ü�� �����͸� ����Ʈ �����ͷ� ��ȯ
	std::shared_ptr<UShapeComponent> pComponent = GetThis<UShapeComponent>();

	// ���� ��ü���� Collision Profile �̸� ���� �۾� ����
	// (���� ������ �ش� ������Ʈ�� �浹 ������ ������Ʈ)
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
	// 1. ��� ������Ʈ(OtherComp)�� ��Ȱ�� ���¶�� �浹 �˻� �ߴ�
	if (false == OtherComp->IsActive())
	{
		return;
	}

	// 2. ���� ������Ʈ�� ��� ������Ʈ ���� �浹 ���θ� �˻�
	//    - FTransform::Collision: �� ������Ʈ�� Transform�� CollisionType�� �̿��� �浹 ���θ� ����
	if (true == FTransform::Collision(CollisionType, Transform, OtherComp->CollisionType, OtherComp->Transform))
	{
		// 2.1 ������ �浹�� ���� ���� ���
		if (false == CollisionCheckSet.contains(OtherComp.get()))
		{
			// 2.1.1 ���� ������Ʈ�� ��� ������Ʈ�� CollisionCheckSet�� �߰�
			CollisionCheckSet.insert(OtherComp.get());
			// ��� ������Ʈ�� ���� ������Ʈ�� ���
			OtherComp->CollisionCheckSet.insert(this);

			// 2.1.2 �浹 ���� �̺�Ʈ(Enter)�� �����Ǿ� ������ ȣ��
			if (nullptr != Enter)
			{
				Enter(this, OtherComp.get()); // �浹 ���� �̺�Ʈ �ݹ� ����
			}
		}
		else
		{
			// 2.2 ������ �̹� �浹�� ���� �ִ� ���
			//     ���� �浹 �̺�Ʈ(Stay)�� �����Ǿ� ������ ȣ��
			if (nullptr != Stay)
			{
				Stay(this, OtherComp.get()); // �浹 ���� �̺�Ʈ �ݹ� ����
			}
		}
	}
	else
	{
		// 3. �浹���� �ʴ� ����
		//    ������ �浹�� ���� �ִٸ� �浹 ���� ó��
		if (true == CollisionCheckSet.contains(OtherComp.get()))
		{
			// 3.1 �浹 ���� �̺�Ʈ(End)�� �����Ǿ� ������ ȣ��
			if (nullptr != End)
			{
				End(this, OtherComp.get()); // �浹 ���� �̺�Ʈ �ݹ� ����
			}

			// 3.2 CollisionCheckSet���� ��� ������Ʈ ����
			CollisionCheckSet.erase(OtherComp.get());
			OtherComp->CollisionCheckSet.erase(this); // ���浵 ���� ������Ʈ�� ����
		}
	}
}


void UShapeComponent::Release()
{
	for (UShapeComponent* Other : CollisionCheckSet)
	{
		Other->CollisionCheckSet.erase(this);
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

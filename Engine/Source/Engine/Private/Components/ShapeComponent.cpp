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
		MSGASSERT("존재하지 않는 그룹과 충돌할수 없습니다 " + std::string(UpperName));
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
	// 현재 설정된 Collision Profile 이름과 동일하다면 작업을 수행하지 않음
	if (NewProfileName == GetCollisionProfileName())
	{
		return;
	}

	// 전달받은 Collision Profile 이름을 대문자로 변환
	std::string UpperName = UEngineString::ToUpper(NewProfileName);

	// 이전 Collision Profile 이름을 저장 (변경 전의 이름)
	std::string PrevProfileName = GetCollisionProfileName();

	// ProfileName 멤버 변수에 새로운 이름을 설정
	ProfileName = UpperName;

	// 컴포넌트가 소속된 레벨 정보를 가져옴
	ULevel* Level = GetOwner()->GetWorld();

	// 현재 객체의 포인터를 스마트 포인터로 변환
	std::shared_ptr<UShapeComponent> pComponent = GetThis<UShapeComponent>();

	// 레벨 객체에서 Collision Profile 이름 변경 작업 수행
	// (레벨 내에서 해당 컴포넌트의 충돌 설정을 업데이트)
	Level->ChangeCollisionProfileName(ProfileName, PrevProfileName, pComponent);
}

bool UShapeComponent::CollisionCheck(std::string_view OtherName, std::vector<UShapeComponent*>& ShapeVector)
{
	std::string UpperName = UEngineString::ToUpper(OtherName);

	std::map<std::string, std::list<std::shared_ptr<UShapeComponent>>>& Collision = GetWorld()->ShapeCompMap;

	if (false == Collision.contains(UpperName))
	{
		MSGASSERT("존재하지 않는 그룹과 충돌할수 없습니다" + std::string(UpperName));
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
	// 1. 상대 컴포넌트(OtherComp)가 비활성 상태라면 충돌 검사 중단
	if (false == OtherComp->IsActive())
	{
		return;
	}

	// 2. 현재 컴포넌트와 상대 컴포넌트 간의 충돌 여부를 검사
	//    - FTransform::Collision: 두 컴포넌트의 Transform과 CollisionType을 이용해 충돌 여부를 판정
	if (true == FTransform::Collision(CollisionType, Transform, OtherComp->CollisionType, OtherComp->Transform))
	{
		// 2.1 이전에 충돌한 적이 없는 경우
		if (false == CollisionCheckSet.contains(OtherComp.get()))
		{
			// 2.1.1 현재 컴포넌트를 상대 컴포넌트의 CollisionCheckSet에 추가
			CollisionCheckSet.insert(OtherComp.get());
			// 상대 컴포넌트도 현재 컴포넌트를 기억
			OtherComp->CollisionCheckSet.insert(this);

			// 2.1.2 충돌 진입 이벤트(Enter)가 설정되어 있으면 호출
			if (nullptr != Enter)
			{
				Enter(this, OtherComp.get()); // 충돌 시작 이벤트 콜백 실행
			}
		}
		else
		{
			// 2.2 이전에 이미 충돌한 적이 있는 경우
			//     지속 충돌 이벤트(Stay)가 설정되어 있으면 호출
			if (nullptr != Stay)
			{
				Stay(this, OtherComp.get()); // 충돌 지속 이벤트 콜백 실행
			}
		}
	}
	else
	{
		// 3. 충돌하지 않는 상태
		//    이전에 충돌한 적이 있다면 충돌 종료 처리
		if (true == CollisionCheckSet.contains(OtherComp.get()))
		{
			// 3.1 충돌 종료 이벤트(End)가 설정되어 있으면 호출
			if (nullptr != End)
			{
				End(this, OtherComp.get()); // 충돌 종료 이벤트 콜백 실행
			}

			// 3.2 CollisionCheckSet에서 상대 컴포넌트 제거
			CollisionCheckSet.erase(OtherComp.get());
			OtherComp->CollisionCheckSet.erase(this); // 상대방도 현재 컴포넌트를 제거
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
		MSGASSERT("아직 충돌 그룹이 지정되지 않은 충돌체를 이벤트 등록하려고 했습니다");
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
		MSGASSERT("아직 충돌 그룹이 지정되지 않은 충돌체를 이벤트 등록하려고 했습니다");
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
		MSGASSERT("아직 충돌 그룹이 지정되지 않은 충돌체를 이벤트 등록하려고 했습니다");
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

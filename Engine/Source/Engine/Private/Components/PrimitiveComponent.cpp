#include "pch.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"

#include "Engine/Classes/Engine/Level.h"
#include "Engine/Classes/Engine/Texture.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Engine/PaperSprite.h"
#include "Engine/Classes/Engine/StaticMesh.h"

#include "Rendering/Buffer/EngineVertex.h"
#include "Rendering/EngineBlend.h"

#include "Core/Public/Misc/DirectoryHelper.h"
#include "Core/Public/Misc/FileHelper.h"
#include "Core/Public/Containers/EngineString.h"
#include "Engine/Classes/Engine/GameEngine.h"

UPrimitiveComponent::UPrimitiveComponent()
{
	//GEngine->GetPhysicsSubSystem()->PrimitiveComponentList.push_front(this);
}

UPrimitiveComponent::~UPrimitiveComponent()
{

}

//void UPrimitiveComponent::UpdateCameraTransform(UCameraComponent* CameraComponent)
//{
//	FTransform& CameraTransform = CameraComponent->GetComponentTransformRef();
//	FTransform& ComponentTransform = GetComponentTransformRef();
//
//	ComponentTransform.View = CameraTransform.View;
//	ComponentTransform.Projection = CameraTransform.Projection;
//	ComponentTransform.WVP = ComponentTransform.World * ComponentTransform.View * ComponentTransform.Projection;
//}

void UPrimitiveComponent::BeginPlay()
{
	USceneComponent::BeginPlay();

	SetOrder(GetOrder());

}

void UPrimitiveComponent::TickComponent(float DeltaTime)
{
	USceneComponent::TickComponent(DeltaTime);

	if (FVector::ZERO != Velocity)
	{
		MoveComponent(Velocity * DeltaTime);
	}
}

void UPrimitiveComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	this->UpdateCameraTransform(CameraComponent);

	for (size_t i = 0; i < RenderUnitVector.size(); i++)
	{
		RenderUnitVector[i].Render(CameraComponent, DeltaTime);
	}

}

URenderAsset& UPrimitiveComponent::CreateRenderUnit()
{
	URenderAsset& NewUnit = RenderUnitVector.emplace_back();
	NewUnit.SetPrimitiveComponent(this);
	return NewUnit;
}

URenderAsset& UPrimitiveComponent::GetRenderUnit(UINT Index)
{
	return RenderUnitVector[Index];
}

void UPrimitiveComponent::SetMesh(std::string_view Name, UINT Index)
{
	URenderAsset& RenderUnit = GetRenderUnit(Index);
	RenderUnit.SetMesh(Name);
}

void UPrimitiveComponent::SetMaterial(std::string_view Name, UINT Index/* = 0 */)
{
	URenderAsset& RenderUnit = GetRenderUnit(Index);
	RenderUnit.SetMaterial(Name);
}


void UPrimitiveComponent::SetOrder(int NewOrder)
{
	if (0 != GetOrder() && NewOrder == GetOrder())
	{
		return;
	}
	int PrevOrder = GetOrder();
	UObject::SetOrder(NewOrder);

	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<UPrimitiveComponent> RendererPtr = GetThis<UPrimitiveComponent>();

	Level->ChangeRenderGroup(0, PrevOrder, RendererPtr);
}

bool UPrimitiveComponent::MoveComponent(const FVector& Delta, const FVector& NewRotation/* = FVector::ZERO */, bool bSweep/* = true */)
{
	// 회전 값이 제공된 경우, 새로운 회전 값을 적용
	if (false == NewRotation.IsZero())
	{
		SetWorldRotation(NewRotation);
	}

	// Sweep이 필요할 경우, SweepCollision을 적용해 충돌을 체크
	if (true == bSweep)
	{
		// 컴포넌트 이동 경로에 대해 충돌을 검사
		SetWorldLocation(SweepCollision(GetWorldLocation(), Delta));

		return true;
	}
	else
	{
		// 컴포넌트가 이동할 새 위치를 계산
		FVector NewLocation = GetWorldLocation() + Delta;

		// Sweep이 없으면 단순히 위치만 업데이트
		SetWorldLocation(NormalComponent(GetWorldLocation(), Delta));

		return true;
	}
}

FVector UPrimitiveComponent::SweepCollision(const FVector& Location, const FVector& Delta)
{
	return GEngine->GetPhysicsSubSystem()->SweepCollision(Location, Delta);
}

FVector UPrimitiveComponent::NormalComponent(const FVector& Location, const FVector& Delta)
{
	return GEngine->GetPhysicsSubSystem()->NormalComponent(Location, Delta);
}

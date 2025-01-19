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

bool UPrimitiveComponent::MoveComponent(const FVector& Delta, const FVector& NewRotation, bool bSweep)
{
	// ȸ�� ���� ������ ���, ���ο� ȸ�� ���� ����
	if (false == NewRotation.IsZero())
	{
		SetWorldRotation(NewRotation);
	}

	// Sweep�� �ʿ��� ���, SweepCollision�� ������ �浹�� üũ
	if (true == bSweep)
	{
		// ������Ʈ �̵� ��ο� ���� �浹�� �˻�
		SetWorldLocation(SweepComponent(GetWorldLocation(), Delta));

		return true;
	}
	else
	{
		// ������Ʈ�� �̵��� �� ��ġ�� ���
		FVector NewLocation = GetWorldLocation() + Delta;

		// Sweep�� ������ �ܼ��� ��ġ�� ������Ʈ
		SetWorldLocation(NewLocation);

		return true;
	}
}

FVector UPrimitiveComponent::SweepComponent(const FVector& Location, const FVector& Delta)
{
	return GEngine->GetPhysicsSubSystem()->SweepComponent(Location, Delta);
}

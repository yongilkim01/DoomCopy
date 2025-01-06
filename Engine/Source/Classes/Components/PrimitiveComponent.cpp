#include "pch.h"
#include "PrimitiveComponent.h"

#include "Classes/Engine/Level.h"
#include "Classes/Engine/Texture.h"
#include "Classes/Camera/CameraComponent.h"
#include "Classes/Engine/PaperSprite.h"
#include "Classes/Engine/StaticMesh.h"

#include "Rendering/Buffer/EngineVertex.h"
#include "Rendering/EngineBlend.h"

#include "Core/Misc/DirectoryHelper.h"
#include "Core/Misc/FileHelper.h"
#include "Core/Containers/EngineString.h"
#include "Core/EngineCore.h"

UPrimitiveComponent::UPrimitiveComponent()
{
}

UPrimitiveComponent::~UPrimitiveComponent()
{

}

void UPrimitiveComponent::BeginPlay()
{
	USceneComponent::BeginPlay();

	SetOrder(GetOrder());

}

void UPrimitiveComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	FTransform& CameraTransform = CameraComponent->GetComponentTransformRef();
	FTransform& ComponentTransform = GetComponentTransformRef();

	ComponentTransform.View = CameraTransform.View;
	ComponentTransform.Projection = CameraTransform.Projection;
	ComponentTransform.WVP = ComponentTransform.World * ComponentTransform.View * ComponentTransform.Projection;

	for (size_t i = 0; i < RenderUnitVector.size(); i++)
	{
		RenderUnitVector[i].Render(CameraComponent, DeltaTime);
	}

}

URenderUnit& UPrimitiveComponent::CreateRenderUnit()
{
	URenderUnit& NewUnit = RenderUnitVector.emplace_back();
	NewUnit.SetPrimitiveComponent(this);
	return NewUnit;
}

URenderUnit& UPrimitiveComponent::GetRenderUnit(UINT Index)
{
	return RenderUnitVector[Index];
}

void UPrimitiveComponent::SetMesh(std::string_view Name, UINT Index)
{
	URenderUnit& RenderUnit = GetRenderUnit(Index);
	RenderUnit.SetMesh(Name);
}

void UPrimitiveComponent::SetMaterial(std::string_view Name, UINT Index)
{
	URenderUnit& RenderUnit = GetRenderUnit(Index);
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
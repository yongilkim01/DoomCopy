#include "pch.h"
#include "PrimitiveComponent.h"

#include "Classes/Engine/Level.h"
#include "Classes/Engine/Texture.h"
#include "Classes/Camera/CameraComponent.h"
#include "Classes/Engine/PaperSprite.h"
#include "Classes/Engine/StaticMesh.h"

#include "Rendering/EngineVertex.h"
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
	for (size_t i = 0; i < Units.size(); i++)
	{
		Units[i].Render(CameraComponent, DeltaTime);
	}

}

URenderUnit& UPrimitiveComponent::CreateRenderUnit()
{
	// TODO: 여기에 return 문을 삽입합니다.
	return Units.emplace_back();
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
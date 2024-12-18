#include "pch.h"
#include "Renderer.h"

#include "Classes/Engine/Level.h"

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
}

void URenderer::BeginPlay()
{
	SetOrder(0);
}

void URenderer::Render(float DeltaTime)
{
	// TODO: Rendering pipeline
}

void URenderer::SetOrder(int NewOrder)
{
	int PrevOrder = GetOrder();
	UObject::SetOrder(NewOrder);
	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<URenderer> RendererPtr = GetThis<URenderer>();
	Level->ChangeRenderGroup(PrevOrder, RendererPtr);
}
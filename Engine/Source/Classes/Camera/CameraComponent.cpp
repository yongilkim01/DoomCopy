#include "pch.h"
#include "CameraComponent.h"

#include "Core/EngineCore.h"
#include "Classes/Engine/Level.h"
#include "Renderer/Renderer.h"

UCameraComponent::UCameraComponent()
{
}

UCameraComponent::~UCameraComponent()
{
}

void UCameraComponent::BeginPlay()
{
	FVector ScreenScale = UEngineCore::GetSceenScale();
	ProjectionScale = ScreenScale;
}

void UCameraComponent::Tick(float DeltaTime)
{
	Transform.View;
	Transform.Projection;
}

void UCameraComponent::Render(float DeltaTime)
{
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Rendereres)
	{
		std::list<std::shared_ptr<URenderer>>& RendererList = RenderGroup.second;
		for (std::shared_ptr<URenderer> Renderer : RendererList)
		{
			Renderer->Render(this, DeltaTime);
		}
	}
}

void UCameraComponent::CalculateViewAndProjection()
{
	FTransform& Transform = GetComponentTransformRef();
	Transform.View.View(Transform.World.ArrVector[3], Transform.World.GetFoward(), Transform.World.GetUp());
	Transform.Projection.OrthographicLH(ProjectionScale.X, ProjectionScale.Y, Near, Far);
}

void UCameraComponent::ChangeRenderGroup(int PrevGroupOrder, std::shared_ptr<URenderer> Renderer)
{
	Rendereres[PrevGroupOrder].remove(Renderer);
	Rendereres[Renderer->GetOrder()].push_back(Renderer);
}

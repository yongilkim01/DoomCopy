#include "pch.h"
#include "CameraComponent.h"

#include "Core/EngineCore.h"
#include "Classes/Engine/Level.h"
#include "Classes/Components/PrimitiveComponent.h"

UCameraComponent::UCameraComponent()
{
}

UCameraComponent::~UCameraComponent()
{
	RenderComponentMap.clear();
}

void UCameraComponent::BeginPlay()
{
	FVector ScreenScale = UEngineCore::GetSceenScale();
	ProjectionScale = ScreenScale;

	ViewPortInfo.Width = UEngineCore::GetSceenScale().X;
	ViewPortInfo.Height = UEngineCore::GetSceenScale().Y;
	ViewPortInfo.TopLeftX = 0.0f;
	ViewPortInfo.TopLeftY = 0.0f;
	ViewPortInfo.MinDepth = 0.0f;
	ViewPortInfo.MaxDepth = 1.0f;
}

void UCameraComponent::Tick(float DeltaTime)
{
	Transform.View;
	Transform.Projection;
}

void UCameraComponent::Render(float DeltaTime)
{
	UEngineCore::GetDevice().GetDeviceContext()->RSSetViewports(1, &ViewPortInfo);

	for (std::pair<const int, std::list<std::shared_ptr<UPrimitiveComponent>>>& RenderGroup : RenderComponentMap)
	{
		std::list<std::shared_ptr<UPrimitiveComponent>>& RendererList = RenderGroup.second;

		if (true == RenderComponentZSort[RenderGroup.first])
		{
			RendererList.sort([](std::shared_ptr<UPrimitiveComponent>& Left, std::shared_ptr<UPrimitiveComponent>& Right)
				{
					return Left->GetComponentTransformRef().WorldLocation.Z > Right->GetComponentTransformRef().WorldLocation.Z;
				});
		}

		for (std::shared_ptr<UPrimitiveComponent> Renderer : RendererList)
		{
			if (false == Renderer->IsActive())
			{
				continue;
			}

			Renderer->Render(this, DeltaTime);
		}
	}
}

void UCameraComponent::Release(float DeltaTime)
{
	for (std::pair<const int, std::list<std::shared_ptr<UPrimitiveComponent>>>& RenderGroup : RenderComponentMap)
	{
		std::list<std::shared_ptr<UPrimitiveComponent>>& RenderList = RenderGroup.second;
		std::list<std::shared_ptr<UPrimitiveComponent>>::iterator StartIter = RenderList.begin();
		std::list<std::shared_ptr<UPrimitiveComponent>>::iterator EndIter = RenderList.end();

		for (; StartIter != EndIter; )
		{
			if (false == (*StartIter)->IsDestroy())
			{
				++StartIter;
				continue;
			}

			StartIter = RenderList.erase(StartIter);
		}
	}
}

void UCameraComponent::CalculateViewAndProjection()
{
	FTransform& Transform = GetComponentTransformRef();
	Transform.View.View(Transform.World.ArrVector[3], Transform.World.GetFoward(), Transform.World.GetUp());

	switch (ProjectionType)
	{
	case EProjectionType::Perspective:
		Transform.Projection.PerspectiveFovDeg(FOV, ProjectionScale.X, ProjectionScale.Y, Near, Far);
		break;
	case EProjectionType::Orthographic:
		Transform.Projection.OrthographicLH(ProjectionScale.X, ProjectionScale.Y, Near, Far);
		break;
	default:
		break;
	}
}

void UCameraComponent::SetZSort(int NewOrder, bool bValue)
{
	RenderComponentZSort[NewOrder] = bValue;
}

void UCameraComponent::ChangeRenderGroup(int PrevGroupOrder, std::shared_ptr<UPrimitiveComponent> Renderer)
{
	RenderComponentMap[PrevGroupOrder].remove(Renderer);
	RenderComponentMap[Renderer->GetOrder()].push_back(Renderer);
}

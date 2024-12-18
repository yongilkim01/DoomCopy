#include "pch.h"
#include "Level.h"
#include "GameFramework/Actor.h"
#include "Renderer/Renderer.h"

ULevel::ULevel()
{

}

ULevel::~ULevel()
{

}

void ULevel::LevelChangeStart()
{
}

void ULevel::LevelChangeEnd()
{
}

void ULevel::Tick(float DeltaTime)
{
	std::list<std::shared_ptr<AActor>>::iterator StartIter = BeginPlayList.begin();
	std::list<std::shared_ptr<AActor>>::iterator EndIter = BeginPlayList.end();

	for (; StartIter != EndIter; )
	{
		std::shared_ptr<AActor> CurActor = *StartIter;

		if (false == CurActor->IsActive())
		{
			++StartIter;
			continue;
		}

		StartIter = BeginPlayList.erase(StartIter);

		CurActor->BeginPlay();
		AllActorList.push_back(CurActor);
	}

	for (std::shared_ptr<AActor> CurActor : AllActorList)
	{
		CurActor->Tick(DeltaTime);
	}
}

void ULevel::Render(float DeltaTime)
{
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RendererGroup : RendererMap)
	{
		std::list<std::shared_ptr<URenderer>>& RendererList = RendererGroup.second;

		for (std::shared_ptr<URenderer> Renderer : RendererList)
		{
			Renderer->Render(DeltaTime);
		}
	}
}

void ULevel::ChangeRenderGroup(int PrevGroupOrder, std::shared_ptr<URenderer> Renderer)
{
	RendererMap[PrevGroupOrder].remove(Renderer);
	RendererMap[Renderer->GetOrder()].push_back(Renderer);
}

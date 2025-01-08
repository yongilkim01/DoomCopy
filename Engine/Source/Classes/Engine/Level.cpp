#include "pch.h"
#include "Level.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/ShapeComponent.h"
#include "Classes/Components/PrimitiveComponent.h"
#include "Rendering/EngineGraphicDevice.h"

#include "Classes/Camera/CameraActor.h"
#include "Classes/Camera/CameraComponent.h"

#include "Tools/DebugGUI/EngineGUI.h"

ULevel::ULevel()
{
	SpawnCamera(0);
}

ULevel::~ULevel()
{
	BeginPlayList.clear();
	AllActorList.clear();
	Cameraes.clear();
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
		if (false == CurActor->IsActive())
		{
			continue;
		}

		CurActor->Tick(DeltaTime);
	}
}

void ULevel::Render(float DeltaTime)
{
	UEngineCore::GetDevice().RenderStart();

	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameraes)
	{
		Camera.second->Tick(DeltaTime);
		Camera.second->GetCameraComponent()->Render(DeltaTime);
	}

	if (true == UEngineWindow::IsApplicationOn())
	{
		UEngineGUI::GUIRender(this);
	}

	UEngineCore::GetDevice().RenderEnd();
}

void ULevel::Release(float DeltaTime)
{
	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameraes)
	{
		Camera.second->GetCameraComponent()->Release(DeltaTime);
	}

	{
		for (std::pair<const std::string_view, std::list<std::shared_ptr<UShapeComponent>>>& Group : ShapeCompMap)
		{
			std::list<std::shared_ptr<UShapeComponent>>& List = Group.second;

			std::list<std::shared_ptr<UShapeComponent>>::iterator StartIter = List.begin();
			std::list<std::shared_ptr<UShapeComponent>>::iterator EndIter = List.end();

			for (; StartIter != EndIter; )
			{
				if (false == (*StartIter)->IsDestroy())
				{
					++StartIter;
					continue;
				}

				StartIter = List.erase(StartIter);
			}
		}
	}

	{
		std::list<std::shared_ptr<AActor>>& List = AllActorList;
		std::list<std::shared_ptr<AActor>>::iterator StartIter = List.begin();
		std::list<std::shared_ptr<AActor>>::iterator EndIter = List.end();

		for (; StartIter != EndIter; )
		{
			if (false == (*StartIter)->IsDestroy())
			{
				++StartIter;
				continue;
			}

			StartIter = List.erase(StartIter);
		}
	}
}

std::shared_ptr<ACameraActor> ULevel::SpawnCamera(int CameraOrder)
{
	std::shared_ptr<ACameraActor> Camera = std::make_shared<ACameraActor>();
	if (true == Cameraes.contains(CameraOrder))
	{
		MSGASSERT("동일한 번호의 카메라가 이미 존재합니다");
	}

	Camera->BeginPlay();

	Cameraes.insert({ CameraOrder , Camera });
	return Camera;
}

void ULevel::ChangeRenderGroup(int CameraOrder, int PrevGroupOrder, std::shared_ptr<UPrimitiveComponent> Renderer)
{
	if (false == Cameraes.contains(CameraOrder))
	{
		MSGASSERT("존재하지 않는 카메라에 랜더러를 집어넣으려고 했습니다.");
		return;
	}
	std::shared_ptr<ACameraActor> Camera = Cameraes[CameraOrder];
	Camera->GetCameraComponent()->ChangeRenderGroup(PrevGroupOrder, Renderer);

}

void ULevel::CreateCollisionProfileName(std::string_view ProfileName)
{
	ShapeCompMap[ProfileName];
}

void ULevel::ChangeCollisionProfileName(std::string_view ProfileName, std::string_view PrevProfileName, std::shared_ptr<UShapeComponent> ShapeComponent)
{
	if (false == ShapeCompMap.contains(ProfileName))
	{
		MSGASSERT("존재하지 않는 콜리전 그룹입니다");
		return;
	}

	if (PrevProfileName != "")
	{
		std::list<std::shared_ptr<UShapeComponent>>& PrevShapeCompList = ShapeCompMap[PrevProfileName];
		PrevShapeCompList.remove(ShapeComponent);
	}

	std::list<std::shared_ptr<UShapeComponent>>& ShapeCompList = ShapeCompMap[ProfileName];
	ShapeCompList.push_back(ShapeComponent);
}

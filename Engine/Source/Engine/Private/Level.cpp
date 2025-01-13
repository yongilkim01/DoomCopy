#include "pch.h"
#include "Engine/Classes/Engine/Level.h"

#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Components/ShapeComponent.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Classes/Camera/CameraActor.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "Rendering/EngineGraphicDevice.h"
#include "Rendering/Public/RenderTarget/RenderTarget.h"

#include "Tools/DebugGUI/EngineGUI.h"

ULevel::ULevel()
{
	SpawnCamera(EEngineCameraType::MainCamera);
	SpawnCamera(EEngineCameraType::UICamera);

	FinalRenderTarget = std::make_shared<URenderTarget>();
	FinalRenderTarget->CreateTarget(UEngineCore::GetSceenScale());
	FinalRenderTarget->CreateDepthStencil();
}

ULevel::~ULevel()
{
	BeginPlayList.clear();
	AllActorList.clear();
	Cameraes.clear();
}

void ULevel::LevelChangeStart()
{
	for (std::shared_ptr<AActor> Actor : BeginPlayList)
	{
		Actor->LevelChangeStart();
	}

	for (std::shared_ptr<AActor> Actor : AllActorList)
	{
		Actor->LevelChangeStart();
	}
}

void ULevel::LevelChangeEnd()
{
	for (std::shared_ptr<AActor> Actor : BeginPlayList)
	{
		Actor->LevelChangeEnd();
	}

	for (std::shared_ptr<AActor> Actor : AllActorList)
	{
		Actor->LevelChangeEnd();
	}
}

void ULevel::Tick(float DeltaTime)
{
	if (GetMainCamera()->IsFreeCamera())
	{
		return;
	}

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

		if (nullptr != CurActor->Parent)
		{
			continue;
		}

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

	FinalRenderTarget->Clear();

	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameraes)
	{
		Camera.second->Tick(DeltaTime);
		Camera.second->GetCameraComponent()->Render(DeltaTime);
		Camera.second->GetCameraComponent()->CameraRenderTarget->MergeRenderTarget(FinalRenderTarget);
	}

	std::shared_ptr<URenderTarget> BackBufferRenderTarget = UEngineCore::GetDevice().GetBackBufferRenderTarget();
	FinalRenderTarget->MergeRenderTarget(BackBufferRenderTarget);

	{
		std::shared_ptr<ACameraActor> Camera = GetMainCamera();
		// 충돌체 릴리즈
		for (std::pair<const std::string, std::list<std::shared_ptr<UShapeComponent>>>& Group : ShapeCompMap)
		{
			std::list<std::shared_ptr<UShapeComponent>>& List = Group.second;
			for (std::shared_ptr<UShapeComponent>& _Collision : List)
			{
				if (false == _Collision->IsActive())
				{
					continue;
				}
				_Collision->DebugRender(Camera->GetCameraComponent().get(), DeltaTime);
			}
		}
	}


	if (true == UEngineWindow::IsApplicationOn())
	{
		UEngineGUI::GUIRender(this);
	}

	UEngineCore::GetDevice().RenderEnd();
}

void ULevel::Collision(float DeltaTime)
{
	for (std::pair<const std::string, std::list<std::string>>& Links : CollisionLinkMap)
	{
		const std::string& LeftProfile = Links.first;
		std::list<std::string>& LinkSecond = Links.second;

		for (std::string& RightProfile : LinkSecond)
		{
			std::list<std::shared_ptr<UShapeComponent>>& LeftList = CheckShapeCompMap[LeftProfile];
			std::list<std::shared_ptr<UShapeComponent>>& RightList = CheckShapeCompMap[RightProfile];

			for (std::shared_ptr<UShapeComponent>& LeftCollision : LeftList)
			{
				for (std::shared_ptr<UShapeComponent>& RightCollision : RightList)
				{
					if (false == LeftCollision->IsActive())
					{
						continue;
					}

					LeftCollision->CollisionEventCheck(RightCollision);
				}
			}
		}
	}
}

void ULevel::Release(float DeltaTime)
{
	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameraes)
	{
		Camera.second->GetCameraComponent()->Release(DeltaTime);
	}

	{
		for (std::pair<const std::string, std::list<std::shared_ptr<UShapeComponent>>>& Group : ShapeCompMap)
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
			if (nullptr != (*StartIter)->Parent)
			{
				// 부모가 있는 애는 어차피 부모가 다 tick
				// 레벨이 돌려줄필요가 없어졌다.
				StartIter = List.erase(StartIter);
				continue;
			}

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

void ULevel::InitLevel(AGameMode* InitGameMode, APawn* InitPawn)
{
	GameMode = InitGameMode;
	MainPawn = InitPawn;
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

//void ULevel::CreateCollisionProfileName(std::string_view ProfileName)
//{
//	std::string UpperName = UEngineString::ToUpper(ProfileName);
//
//	ShapeCompMap[UpperName];
//}

void ULevel::PushCollisionProfileName(std::shared_ptr<UPrimitiveComponent> PrComp)
{
}

void ULevel::LinkCollisionProfile(std::string_view LeftProfileName, std::string_view RightProfileName)
{
	std::string LeftUpperName = UEngineString::ToUpper(LeftProfileName);
	std::string RightUpperName = UEngineString::ToUpper(RightProfileName);

	CollisionLinkMap[LeftUpperName].push_back(RightUpperName);
}

void ULevel::ChangeCollisionProfileName(std::string_view ProfileName, std::string_view PrevProfileName, std::shared_ptr<UShapeComponent> ShapeComponent)
{
	if (false == ShapeCompMap.contains(ProfileName.data()))
	{
		MSGASSERT("존재하지 않는 콜리전 그룹입니다");
		return;
	}

	std::string PrevUpperName = UEngineString::ToUpper(ProfileName);


	if (PrevProfileName != "")
	{
		std::list<std::shared_ptr<UShapeComponent>>& PrevShapeCompList = ShapeCompMap[PrevUpperName];
		PrevShapeCompList.remove(ShapeComponent);
	}

	std::string UpperName = UEngineString::ToUpper(ProfileName);
	std::list<std::shared_ptr<UShapeComponent>>& ShapeCompList = ShapeCompMap[UpperName];

	ShapeCompList.push_back(ShapeComponent);
}

void ULevel::CreateCollisionProfile(std::string_view ProfileName)
{
	std::string UpperName = UEngineString::ToUpper(ProfileName);

	ShapeCompMap[UpperName];
}

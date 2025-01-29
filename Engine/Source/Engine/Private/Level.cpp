#include "Engine.h"
#include "Engine/Classes/Engine/Level.h"

#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Classes/GameFramework/HUD.h"
#include "Engine/Classes/Engine/Font.h"
#include "Engine/Classes/Components/ShapeComponent.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Classes/Components/LightComponent.h"
#include "Engine/Classes/Camera/CameraActor.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "Rendering/Public/Device/GraphicDevice.h"
#include "Rendering/Public/RenderTarget/RenderTarget.h"

#include "Editor/Public/EngineGUI.h"

#include "UI/Public/Widget/Widget.h"

ULevel::ULevel()
{
	SpawnCamera(EEngineCameraType::MainCamera);

	std::shared_ptr<ACameraActor> UICamera = SpawnCamera(EEngineCameraType::UICamera);
	UICamera->GetCameraComponent()->SetProjectionType(EProjectionType::Orthographic);

	FinalRenderTarget = std::make_shared<URenderTarget>();
	FinalRenderTarget->CreateTarget(UGameEngine::GetSceenScale());
	FinalRenderTarget->SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
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
	UGameEngine::StartRender();

	FinalRenderTarget->Clear();

	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameraes)
	{
		if (Camera.first == static_cast<int>(EEngineCameraType::UICamera))
		{
			continue;
		}

		if (false == Camera.second->IsActive())
		{
			continue;
		}

		LightDatas.Count = 0;

		for (size_t i = 0; i < LightComponentVector.size(); i++)
		{
			LightComponentVector[i]->UpdateLight(Camera.second->GetCameraComponent().get(), DeltaTime);
			++LightDatas.Count;
			LightDatas.LightDataArr[i] = LightComponentVector[i]->LightData;
		}

		Camera.second->Tick(DeltaTime);
		Camera.second->GetCameraComponent()->Render(DeltaTime);

		// Camera.second->GetCameraComponent()->CameraRenderTarget->MergeRenderTarget(FinalRenderTarget);
	}

	if (true == Cameraes.contains(static_cast<int>(EEngineCameraType::UICamera)))
	{
		std::shared_ptr<ACameraActor> CameraActor = Cameraes[static_cast<int>(EEngineCameraType::UICamera)];

		if (true == CameraActor->IsActive())
		{
			std::shared_ptr<UCameraComponent> CameraComponent = Cameraes[static_cast<int>(EEngineCameraType::UICamera)]->GetCameraComponent();

			CameraActor->Tick(DeltaTime);

			CameraComponent->CameraRenderTarget->Clear();
			CameraComponent->CameraRenderTarget->Setting();

			HUD->RenderUI(CameraComponent.get(), DeltaTime);

			// CameraComponent->CameraRenderTarget->MergeRenderTarget(FinalRenderTarget);
		}
	}
	else
	{
		MSGASSERT("UI카메라가 존재하지 않습니다");
	}

	Cameraes[static_cast<int>(EEngineCameraType::MainCamera)]->GetCameraComponent()->CameraRenderTarget->MergeRenderTarget(FinalRenderTarget);
	Cameraes[static_cast<int>(EEngineCameraType::UICamera)]->GetCameraComponent()->CameraRenderTarget->MergeRenderTarget(FinalRenderTarget);

	std::shared_ptr<URenderTarget> BackBufferRenderTarget = UGameEngine::GetBackBufferRenderTarget();
	FinalRenderTarget->MergeRenderTarget(BackBufferRenderTarget);

#ifdef _DEBUG
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
#endif


	if (true == UEngineWindow::IsApplicationOn())
	{
		UEngineGUI::GUIRender(this);
	}

	UGameEngine::EndRender();
}

void ULevel::Collision(float DeltaTime)
{
	// 1. CollisionLinkMap을 순회하며 충돌 검사 대상이 되는 프로파일 간의 관계를 확인
	//    - CollisionLinkMap: 충돌해야 할 콜리전 프로파일 쌍(LeftProfile ↔ RightProfile)을 관리
	for (std::pair<const std::string, std::list<std::string>>& Links : CollisionLinkMap)
	{
		// LeftProfile: 검사 대상이 되는 첫 번째 콜리전 프로파일 이름
		const std::string& LeftProfile = Links.first;

		// LinkSecond: LeftProfile과 충돌 관계에 있는 다른 프로파일 이름들의 리스트
		std::list<std::string>& LinkSecond = Links.second;

		// 2. LeftProfile에 연결된 모든 RightProfile을 순회
		for (std::string& RightProfile : LinkSecond)
		{
			// LeftProfile과 RightProfile에 속하는 ShapeComponent 리스트를 가져옴
			std::list<std::shared_ptr<UShapeComponent>>& LeftList = ShapeCompMap[LeftProfile];
			std::list<std::shared_ptr<UShapeComponent>>& RightList = ShapeCompMap[RightProfile];

			// 3. LeftList와 RightList의 모든 컴포넌트 간에 충돌 검사 수행
			for (std::shared_ptr<UShapeComponent>& LeftCollision : LeftList)
			{
				for (std::shared_ptr<UShapeComponent>& RightCollision : RightList)
				{
					// 3.1 동일한 컴포넌트라면 충돌 검사 생략
					if (LeftCollision == RightCollision)
					{
						continue;
					}

					// 3.2 LeftCollision이 비활성 상태라면 충돌 검사 생략
					if (false == LeftCollision->IsActive())
					{
						continue;
					}

					// 3.3 LeftCollision과 RightCollision의 충돌 여부를 검사하고 이벤트 처리
					//     - CollisionEventCheck를 호출하여 충돌 진입(Enter), 지속(Stay), 종료(End) 처리
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
		// 충돌체 릴리즈
		for (std::pair<const std::string, std::list<std::shared_ptr<UShapeComponent>>>& Group : CheckShapeCompMap)
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

void ULevel::InitLevel(AGameMode* InitGameMode, APawn* InitPawn, AHUD* InitHUD)
{
	GameMode = InitGameMode;
	MainPawn = InitPawn;
	HUD = InitHUD;
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
	// 1. ProfileName이 ShapeCompMap에 존재하는지 확인
	if (false == ShapeCompMap.contains(ProfileName.data()))
	{
		// ProfileName이 존재하지 않을 경우 경고 메시지를 출력하고 작업 중단
		MSGASSERT("존재하지 않는 콜리전 그룹입니다");
		return;
	}

	// 2. 이전 콜리전 프로파일 이름을 대문자로 변환
	std::string PrevUpperName = UEngineString::ToUpper(ProfileName);

	// 3. 이전 프로파일 이름(PrevProfileName)이 비어 있지 않은 경우
	if (PrevProfileName != "")
	{
		// 3.1 이전 프로파일 이름에 해당하는 ShapeComponent 리스트를 가져옴
		std::list<std::shared_ptr<UShapeComponent>>& PrevShapeCompList = ShapeCompMap[PrevUpperName];

		// 3.2 이전 리스트에서 현재 ShapeComponent를 제거
		PrevShapeCompList.remove(ShapeComponent);
	}

	// 4. 새로운 콜리전 프로파일 이름을 대문자로 변환
	std::string UpperName = UEngineString::ToUpper(ProfileName);

	// 5. 새로운 프로파일 이름에 해당하는 ShapeComponent 리스트를 가져옴
	std::list<std::shared_ptr<UShapeComponent>>& ShapeCompList = ShapeCompMap[UpperName];

	// 6. 새로운 리스트에 현재 ShapeComponent를 추가
	ShapeCompList.push_back(ShapeComponent);
}


void ULevel::PushLight(std::shared_ptr<ULightComponent> LightComponent)
{
	LightComponentVector.push_back(LightComponent);
}

void ULevel::CreateCollisionProfile(std::string_view ProfileName)
{
	std::string UpperName = UEngineString::ToUpper(ProfileName);

	ShapeCompMap[UpperName];
}

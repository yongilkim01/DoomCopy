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
		MSGASSERT("UIī�޶� �������� �ʽ��ϴ�");
	}

	Cameraes[static_cast<int>(EEngineCameraType::MainCamera)]->GetCameraComponent()->CameraRenderTarget->MergeRenderTarget(FinalRenderTarget);
	Cameraes[static_cast<int>(EEngineCameraType::UICamera)]->GetCameraComponent()->CameraRenderTarget->MergeRenderTarget(FinalRenderTarget);

	std::shared_ptr<URenderTarget> BackBufferRenderTarget = UGameEngine::GetBackBufferRenderTarget();
	FinalRenderTarget->MergeRenderTarget(BackBufferRenderTarget);

#ifdef _DEBUG
	{
		std::shared_ptr<ACameraActor> Camera = GetMainCamera();
		// �浹ü ������
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
	// 1. CollisionLinkMap�� ��ȸ�ϸ� �浹 �˻� ����� �Ǵ� �������� ���� ���踦 Ȯ��
	//    - CollisionLinkMap: �浹�ؾ� �� �ݸ��� �������� ��(LeftProfile �� RightProfile)�� ����
	for (std::pair<const std::string, std::list<std::string>>& Links : CollisionLinkMap)
	{
		// LeftProfile: �˻� ����� �Ǵ� ù ��° �ݸ��� �������� �̸�
		const std::string& LeftProfile = Links.first;

		// LinkSecond: LeftProfile�� �浹 ���迡 �ִ� �ٸ� �������� �̸����� ����Ʈ
		std::list<std::string>& LinkSecond = Links.second;

		// 2. LeftProfile�� ����� ��� RightProfile�� ��ȸ
		for (std::string& RightProfile : LinkSecond)
		{
			// LeftProfile�� RightProfile�� ���ϴ� ShapeComponent ����Ʈ�� ������
			std::list<std::shared_ptr<UShapeComponent>>& LeftList = ShapeCompMap[LeftProfile];
			std::list<std::shared_ptr<UShapeComponent>>& RightList = ShapeCompMap[RightProfile];

			// 3. LeftList�� RightList�� ��� ������Ʈ ���� �浹 �˻� ����
			for (std::shared_ptr<UShapeComponent>& LeftCollision : LeftList)
			{
				for (std::shared_ptr<UShapeComponent>& RightCollision : RightList)
				{
					// 3.1 ������ ������Ʈ��� �浹 �˻� ����
					if (LeftCollision == RightCollision)
					{
						continue;
					}

					// 3.2 LeftCollision�� ��Ȱ�� ���¶�� �浹 �˻� ����
					if (false == LeftCollision->IsActive())
					{
						continue;
					}

					// 3.3 LeftCollision�� RightCollision�� �浹 ���θ� �˻��ϰ� �̺�Ʈ ó��
					//     - CollisionEventCheck�� ȣ���Ͽ� �浹 ����(Enter), ����(Stay), ����(End) ó��
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
		// �浹ü ������
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
				// �θ� �ִ� �ִ� ������ �θ� �� tick
				// ������ �������ʿ䰡 ��������.
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
		MSGASSERT("������ ��ȣ�� ī�޶� �̹� �����մϴ�");
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
		MSGASSERT("�������� �ʴ� ī�޶� �������� ����������� �߽��ϴ�.");
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
	// 1. ProfileName�� ShapeCompMap�� �����ϴ��� Ȯ��
	if (false == ShapeCompMap.contains(ProfileName.data()))
	{
		// ProfileName�� �������� ���� ��� ��� �޽����� ����ϰ� �۾� �ߴ�
		MSGASSERT("�������� �ʴ� �ݸ��� �׷��Դϴ�");
		return;
	}

	// 2. ���� �ݸ��� �������� �̸��� �빮�ڷ� ��ȯ
	std::string PrevUpperName = UEngineString::ToUpper(ProfileName);

	// 3. ���� �������� �̸�(PrevProfileName)�� ��� ���� ���� ���
	if (PrevProfileName != "")
	{
		// 3.1 ���� �������� �̸��� �ش��ϴ� ShapeComponent ����Ʈ�� ������
		std::list<std::shared_ptr<UShapeComponent>>& PrevShapeCompList = ShapeCompMap[PrevUpperName];

		// 3.2 ���� ����Ʈ���� ���� ShapeComponent�� ����
		PrevShapeCompList.remove(ShapeComponent);
	}

	// 4. ���ο� �ݸ��� �������� �̸��� �빮�ڷ� ��ȯ
	std::string UpperName = UEngineString::ToUpper(ProfileName);

	// 5. ���ο� �������� �̸��� �ش��ϴ� ShapeComponent ����Ʈ�� ������
	std::list<std::shared_ptr<UShapeComponent>>& ShapeCompList = ShapeCompMap[UpperName];

	// 6. ���ο� ����Ʈ�� ���� ShapeComponent�� �߰�
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

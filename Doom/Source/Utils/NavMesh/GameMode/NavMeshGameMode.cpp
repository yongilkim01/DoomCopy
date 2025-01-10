#include "pch.h"
#include "NavMeshGameMode.h"

#include <Classes/Camera/CameraActor.h>
#include <Classes/Camera/CameraComponent.h>
#include <Classes/Components/StaticMeshComponent.h>
#include <Input/EngineInput.h>
#include <Rendering/Buffer/EngineVertex.h>

#include <Tools/DebugGUI/EngineGUI.h>
#include <Tools/DebugGUI/EngineGUIWindow.h>
#include <ThirdParty/imgui/imgui.h>

#include "Utils/NavMesh/Actor/NavMeshCharacter.h"
#include "Utils/NavMesh/Actor/NavMeshMap.h"
#include "Utils/GUI/GUIEditor.h"
#include "Utils/NavMesh/Component/NavMeshComponent.h"
#include "Utils/NavMesh/Actor/NaviMeshManager.h"

class UNavMeshDebugWindow : public UEngineGUIWindow
{
public:
	std::shared_ptr<ANavMeshCharacter> TestPlayer;

	void OnGUI() override
	{

		ImGui::Text(std::to_string(UNaviMeshManager::GetInstance().GetCurNaviDataIndex()).c_str());
		ImGui::Text("Linked Navi Data");
		ImGui::Text(("Distance : " + std::to_string(UNaviMeshManager::GetInstance().Distance)).c_str());
		for (int i = 0; i < UNaviMeshManager::GetInstance().GetPlayerNaviData().LinkNaviDataIndex.size(); i++)
		{
			ImGui::Text(std::to_string(UNaviMeshManager::GetInstance().GetPlayerNaviData().LinkNaviDataIndex[i]).c_str());
		}
		//for(int i = 0; i < UNaviMeshManager::GetInstance().GetNaviDataVector().size(); i++)
		//{
		//	ImGui::Text(("NaviData" + std::to_string(i + 1)).c_str());
		//	ImGui::Text("Linked Navi Data");
		//	for (int j = 0; j < UNaviMeshManager::GetInstance().GetNaviDataByIndex(i).LinkNaviDataIndex.size(); j++)
		//	{
		//		ImGui::Text(std::to_string(UNaviMeshManager::GetInstance().GetNaviDataByIndex(i).LinkNaviDataIndex[j]).c_str());
		//	}
		//}

		//ImGui::Text(std::to_string(UNaviMeshManager::GetInstance().Distance).c_str());

		std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();

		if (true == ImGui::Button("FreeCameraOn"))
		{
			GetWorld()->GetMainCamera()->SwitchFreeCamera();
		}
	}
};

ANavMeshGameMode::ANavMeshGameMode()
{
	{
		GetWorld()->CreateCollisionProfile("Map");
		GetWorld()->CreateCollisionProfile("Player");
	}
	{
		PlayerCharacter = GetWorld()->SpawnActor<ANavMeshCharacter>();
		PlayerCharacter->SetActorLocation(FVector{ -150.0f, 100.0f, 50.0f });
	}
	{
		TestMap = GetWorld()->SpawnActor<ANavMeshMap>();
	}
	{
		Camera = GetWorld()->GetMainCamera();
		Camera->AddActorRelativeLocation({ 0.0f, 80.0f, -500.0f });
		Camera->AddActorRotation({ 10.0f, 0.0f, 0.0f });
		//Camera->AddActorRelativeLocation({ 0.0f, 300.0f, 0.0f });
		//Camera->AddActorRotation({ 90.0f, 0.0f, 0.0f });
		Camera->GetCameraComponent()->SetZSort(0, true);
	}

	std::shared_ptr<UNavMeshDebugWindow> Window = UEngineGUI::CreateGUIWindow<UNavMeshDebugWindow>("NavMeshDebugWindow");
	Window->TestPlayer = PlayerCharacter;

	UNaviMeshManager::GetInstance().Init(PlayerCharacter.get(), TestMap.get(), "");

}

ANavMeshGameMode::~ANavMeshGameMode()
{
}

void ANavMeshGameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	UNaviMeshManager::GetInstance().Tick(DeltaTime);
}

void ANavMeshGameMode::LevelChangeStart()
{
	UEngineGUI::OffAllWindow();

	{
		std::shared_ptr<UGUIEditor> GUIWindow = UEngineGUI::FindGUIWindow<UGUIEditor>("GUIEditor");

		if (nullptr == GUIWindow)
		{
			GUIWindow = UEngineGUI::CreateGUIWindow<UGUIEditor>("GUIEditor");
		}

		GUIWindow->SetActive(true);
	}

	{
		std::shared_ptr<UNavMeshDebugWindow> Window = UEngineGUI::FindGUIWindow<UNavMeshDebugWindow>("NavMeshDebugWindow");

		if (nullptr == Window)
		{
			Window = UEngineGUI::CreateGUIWindow<UNavMeshDebugWindow>("NavMeshDebugWindow");
		}

		Window->SetActive(true);
	}
}


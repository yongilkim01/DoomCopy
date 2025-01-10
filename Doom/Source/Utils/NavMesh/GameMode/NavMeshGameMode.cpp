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
		ImGui::Button("WindowButton");
		ImGui::SameLine(); // ÇÑ°£ ¶ç±â
		ImGui::Text("test");

		for (int i = 0; i < UNaviMeshManager::GetInstance().GetPlayerNaviData().VertexDataVector.size(); i++)
		{
			ImGui::Text(UNaviMeshManager::GetInstance().GetLocationPlayerNaviData(i).ToString().c_str());
		}
		ImGui::Text(std::to_string(UNaviMeshManager::GetInstance().Distance).c_str());

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
		PlayerCharacter->SetActorLocation(FVector{ -150.0f, 1.0f, 50.0f });
	}
	{
		TestMap = GetWorld()->SpawnActor<ANavMeshMap>();
	}
	{
		Camera = GetWorld()->GetMainCamera();
		//Camera->AddActorRelativeLocation({ 0.0f, 80.0f, -500.0f });
		//Camera->AddActorRotation({ 10.0f, 0.0f, 0.0f });
		Camera->AddActorRelativeLocation({ 0.0f, 300.0f, 0.0f });
		Camera->AddActorRotation({ 90.0f, 0.0f, 0.0f });
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

	float Result = UNaviMeshManager::GetInstance().GetPlayerNaviData().Intersect(PlayerCharacter.get(), TestMap.get());

	if(0.1f < Result && 1.0f > Result)
	{
		//PlayerCharacter->AddActorLocation({ 0.0f, -0.5f, 0.0f });
	}
	else
	{
		//PlayerCharacter->AddActorLocation({ 0.0f, -0.5f, 0.0f });
	}


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


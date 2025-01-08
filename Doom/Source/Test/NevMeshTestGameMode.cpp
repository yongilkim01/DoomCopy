#include "pch.h"
#include "NevMeshTestGameMode.h"

#include "TestPlayer.h"
#include "TestMap.h"

#include <Classes/Camera/CameraActor.h>
#include <Classes/Camera/CameraComponent.h>
#include <Classes/Components/StaticMeshComponent.h>
#include <Input/EngineInput.h>

#include <Tools/DebugGUI/EngineGUI.h>
#include <Tools/DebugGUI/EngineGUIWindow.h>
#include <ThirdParty/imgui/imgui.h>

class TestDebugWindow : public UEngineGUIWindow
{
public:
	std::shared_ptr<ATestPlayer> TestPlayer;

	void OnGUI() override
	{
		ImGui::Button("WindowButton");
		ImGui::SameLine(); // ÇÑ°£ ¶ç±â
		ImGui::Text("test");

		ImGui::Text(TestPlayer->GetActorLocation().ToString().c_str());
		ImGui::Text(std::to_string(TestPlayer->GetResult()).c_str());

		std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();

		if (true == ImGui::Button("FreeCameraOn"))
		{
			GetWorld()->GetMainCamera()->SwitchFreeCamera();
		}
	}
};

ANevMeshTestGameMode::ANevMeshTestGameMode()
{
	{
		GetWorld()->CreateCollisionProfile("Map");
		GetWorld()->CreateCollisionProfile("Player");
	}
	{
		TestPlayer = GetWorld()->SpawnActor<ATestPlayer>();
		TestPlayer->SetActorLocation(FVector{ 0.0f, 150.0f, 0.0f });
	}
	{
		TestMap = GetWorld()->SpawnActor<ATestMap>();
		TestMap->SetActorRelativeScale3D(FVector{ 300.0f, 300.0f, 300.0f });
		TestMap->SetActorLocation({ 0.0f, 0.0f, 0.0f });
		TestMap->AddActorRotation({ 90.0f, 0.0f, 0.0f });
		//TestMap->AddActorRotation({ 0.0f, 0.0f, 20.0f });
	}
	{
		Camera = GetWorld()->GetMainCamera();
		//Camera->AddActorLocation({ 10.0f, 0.0f, 0.0f });
		Camera->AddActorRelativeLocation({ 0.0f, 30.0f, -200.0f });
		Camera->GetCameraComponent()->SetZSort(0, true);
		//Camera->AttachToActor(TestPlayer.get());
	}

	std::shared_ptr<TestDebugWindow> Window = UEngineGUI::CreateGUIWindow<TestDebugWindow>("TestDebugWindow");
	Window->TestPlayer = TestPlayer;

}

ANevMeshTestGameMode::~ANevMeshTestGameMode()
{
}

void ANevMeshTestGameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}


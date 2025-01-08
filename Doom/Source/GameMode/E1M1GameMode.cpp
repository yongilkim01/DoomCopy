#include "pch.h"
#include "E1M1GameMode.h"

#include "DoomGuy/DoomGuy.h"
#include "Actor/Round1/E1M1Map.h"

#include <Classes/Camera/CameraActor.h>
#include <Classes/Camera/CameraComponent.h>
#include <Classes/Components/StaticMeshComponent.h>
#include <Input/EngineInput.h>

#include <Tools/DebugGUI/EngineGUI.h>
#include <Tools/DebugGUI/EngineGUIWindow.h>
#include <ThirdParty/imgui/imgui.h>


class E1M1DebugWindow : public UEngineGUIWindow
{
public:
	std::shared_ptr<ADoomGuy> DoomGuy;

	void OnGUI() override
	{
		ImGui::Button("WindowButton");
		ImGui::SameLine(); // ÇÑ°£ ¶ç±â
		ImGui::Text("test");

		std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();

		if (true == ImGui::Button("FreeCameraOn"))
		{
			GetWorld()->GetMainCamera()->SwitchFreeCamera();
		}

		ImGui::Text("Pos : %s", DoomGuy->GetActorLocation().ToString().c_str());
	}
};

AE1M1GameMode::AE1M1GameMode()
{
	{
		GetWorld()->CreateCollisionProfile("Monster");
		GetWorld()->CreateCollisionProfile("Player");
	}
	{
		DoomGuy = GetWorld()->SpawnActor<ADoomGuy>();
		DoomGuy->SetActorLocation(FVector{ -1042.0f, 14.0f, 3548.0f });
	}
	{
		E1M1Map = GetWorld()->SpawnActor<AE1M1Map>();
		E1M1Map->SetActorRelativeScale3D(FVector{ 1.0f, 1.0f, 1.0f });
		E1M1Map->SetActorLocation({ 0.0f, 0.0f, 0.0f });
	}
	{
		Camera = GetWorld()->GetMainCamera();
		//Camera->AddActorLocation({ 10.0f, 0.0f, 0.0f });
		Camera->AddActorRelativeLocation({ 0.0f, 10.0f, -100.0f });
		Camera->GetCameraComponent()->SetZSort(0, true);
		Camera->AttachToActor(DoomGuy.get());
	}

	std::shared_ptr<E1M1DebugWindow> Window = UEngineGUI::CreateGUIWindow<E1M1DebugWindow>("E1M1DebugWindow");
	Window->DoomGuy = DoomGuy;
}

AE1M1GameMode::~AE1M1GameMode()
{
}

void AE1M1GameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	//UEngineDebug::OutPutString(Camera->ScreenMouseLocationToWorldLocation().ToString());

	//if (UEngineInput::IsPress('A'))
	//{
	//	Camera->AddActorLocation(FVector{ -2000.0f * DeltaTime, 0.0f, 0.0f });
	//}
	//if (UEngineInput::IsPress('D'))
	//{
	//	Camera->AddActorLocation(FVector{ 2000.0f * DeltaTime, 0.0f, 0.0f });
	//}
	//if (UEngineInput::IsPress('W'))
	//{
	//	Camera->AddActorLocation(FVector{ 0.0f, 0.0f, 2000.0f * DeltaTime, 0.0f });
	//}
	//if (UEngineInput::IsPress('S'))
	//{
	//	Camera->AddActorLocation(FVector{ 0.0f, 0.0f, -2000.0f * DeltaTime, 0.0f });
	//}
	//if (UEngineInput::IsPress(VK_UP))
	//{
	//	Camera->AddActorLocation(FVector{ 0.0f, 2000.0f * DeltaTime, 0.0f, 0.0f });
	//}
	//if (UEngineInput::IsPress(VK_DOWN))
	//{
	//	Camera->AddActorLocation(FVector{ 0.0f, -2000.0f * DeltaTime, 0.0f, 0.0f });
	//}
	//if (UEngineInput::IsPress(VK_LEFT))
	//{
	//	Camera->AddActorRotation(FVector{ -20.0f * DeltaTime, 0.0f, 0.0f });
	//}
	//if (UEngineInput::IsPress(VK_RIGHT))
	//{
	//	Camera->AddActorRotation(FVector{ 20.0f * DeltaTime, 0.0f, 0.0f });
	//}
}
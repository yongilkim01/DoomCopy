#include "pch.h"
#include "Round1GameMode.h"

#include "Actor/Round1/DoomMap.h"

#include "Utils/DrawLineActor.h"
#include "Utils/DrawSquareActor.h"
#include "Componennts/LineComponent.h"

#include <Classes/Camera/CameraActor.h>
#include <Classes/Components/StaticMeshComponent.h>
#include <Input/EngineInput.h>

#include <Tools/DebugGUI/EngineGUI.h>
#include <Tools/DebugGUI/EngineGUIWindow.h>
#include <ThirdParty/imgui/imgui.h>


class Round1Window : public UEngineGUIWindow
{
public:
	void OnGUI() override
	{
		ImGui::Button("WindowButton");
		ImGui::SameLine(); // ÇÑ°£ ¶ç±â
		ImGui::Text("test");
	}
};


ARound1GameMode::ARound1GameMode()
{
	//{
	//	std::shared_ptr<ADrawSquareActor> DrawSquareActor = GetWorld()->SpawnActor<ADrawSquareActor>();
	//	DrawSquareActor->SetActorRelativeScale3D(FVector{ 1.0f, 1.0f, 1.0f });
	//	DrawSquareActorVector.push_back(DrawSquareActor);
	//}


	{
		DrawLineActor = GetWorld()->SpawnActor<ADrawLineActor>();
		//DrawLineActor->SetActorRotation(FVector{ 0.0f, 0.0f, 30.0f });
		DrawLineActor->SetActorRelativeScale3D(FVector{ 1.0f, 1.0f, 1.0f });
		DrawLineActor->SetActorRelativeScale3D({ 2.f, 2.f, 2.f });
		DrawLineActor->SetActorLocation({ 60.0f, 0.f, 0.f });
	}

	Camera = GetWorld()->GetMainCamera();
	Camera->AddActorLocation({ 10.0f, 0.0f, 0.0f });
	Camera->AddActorRelativeLocation({ 0.0f, 0.0f, -500.0f});

	UEngineGUI::CreateGUIWindow<Round1Window>("Round1Window");
}

ARound1GameMode::~ARound1GameMode()
{
}

void ARound1GameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	//UEngineDebug::OutPutString(Camera->ScreenMouseLocationToWorldLocation().ToString());


	if (UEngineInput::IsPress('A'))
	{
		Camera->AddActorLocation(FVector{ -2000.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		Camera->AddActorLocation(FVector{ 2000.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('W'))
	{
		Camera->AddActorLocation(FVector{ 0.0f, 0.0f, 2000.0f * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		Camera->AddActorLocation(FVector{ 0.0f, 0.0f, -2000.0f * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress(VK_UP))
	{
		Camera->AddActorLocation(FVector{ 0.0f, 2000.0f * DeltaTime, 0.0f, 0.0f });
	}	
	if (UEngineInput::IsPress(VK_DOWN))
	{
		Camera->AddActorLocation(FVector{ 0.0f, -2000.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress(VK_LEFT))
	{
		Camera->AddActorRotation(FVector{ -20.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		Camera->AddActorRotation(FVector{ 20.0f * DeltaTime, 0.0f, 0.0f });
	}
}
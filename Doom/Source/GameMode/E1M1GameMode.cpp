#include "pch.h"
#include "E1M1GameMode.h"

#include <Core/Public/Misc/FileHelper.h>
#include <Core/Public/Misc/DirectoryHelper.h>
#include <Core/Public/Serialization/Archive.h>

#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Input/EngineInput.h>
#include <Rendering/Buffer/EngineVertex.h>

#include <NavigationSystem/Public/NavigationSystem.h>
#include <Tools/DebugGUI/EngineGUI.h>
#include <Tools/DebugGUI/EngineGUIWindow.h>
#include <ThirdParty/imgui/imgui.h>

#include "Public/Characters/DoomGuy.h"
#include "Actor/Round1/E1M1Map.h"
#include "Utils/GUI/GUIEditor.h"
#include "Utils/NavMesh/Actor/NavMeshMap.h"


class E1M1DebugWindow : public UEngineGUIWindow
{
public:
	//std::shared_ptr<ADoomGuy> DoomGuy;

	void OnGUI() override
	{
		//ImGui::Button("WindowButton");
		//ImGui::SameLine(); // ÇÑ°£ ¶ç±â
		//ImGui::Text("test");

		std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();

		if (true == ImGui::Button("FreeCameraOn"))
		{
			GetWorld()->GetMainCamera()->SwitchFreeCamera();
		}

		////ImGui::Text("Pos : %s", DoomGuy->GetActorLocation().ToString().c_str());
	}
};

AE1M1GameMode::AE1M1GameMode()
{
	{
		GetWorld()->CreateCollisionProfile("Monster");
		GetWorld()->CreateCollisionProfile("Player");
	}
	{
		E1M1Map = GetWorld()->SpawnActor<AE1M1Map>();
		NavMap = GetWorld()->SpawnActor<ANavMeshMap>();
	}
	{
		DoomGuyCharacter = GetWorld()->SpawnActor<ADoomGuyCharacter>();
		DoomGuyCharacter->SetActorLocation(FVector{ -1042.0f, 14.0f, 3548.0f });
	}
	{
		Camera = GetWorld()->GetMainCamera();
		Camera->AddActorRelativeLocation({ 0.0f, 30.0f, -10.0f });
		Camera->GetCameraComponent()->SetZSort(0, true);
		Camera->AttachToActor(DoomGuyCharacter.get());
	}

	std::shared_ptr<E1M1DebugWindow> Window = UEngineGUI::CreateGUIWindow<E1M1DebugWindow>("E1M1DebugWindow");

	UNavigationSystem::GetInstance().Init(DoomGuyCharacter.get(), NavMap.get(), "");
}

AE1M1GameMode::~AE1M1GameMode()
{
}

void AE1M1GameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

void AE1M1GameMode::LevelChangeStart()
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
		std::shared_ptr<E1M1DebugWindow> Window = UEngineGUI::FindGUIWindow<E1M1DebugWindow>("E1M1DebugWindow");

		if (nullptr == Window)
		{
			Window = UEngineGUI::CreateGUIWindow<E1M1DebugWindow>("E1M1DebugWindow");
		}

		Window->SetActive(true);
	}
}

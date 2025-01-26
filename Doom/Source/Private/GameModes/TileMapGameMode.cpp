#include "Doom.h"
#include "Public/GameModes/TileMapGameMode.h"

#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Platform/Public/Input/EngineInput.h>
#include <Rendering/Public/Buffer/EngineVertex.h>

#include <Tools/DebugGUI/EngineGUI.h>
#include <Tools/DebugGUI/EngineGUIWindow.h>
#include <ThirdParty/imgui/imgui.h>

class UTileMapWindow : public UEngineGUIWindow
{
public:
	void OnGUI() override
	{
		if (true == ImGui::Button("WindowButton"))
		{
		}
		if (true == ImGui::Button("FreeCameraOn"))
		{
		}
		ImGui::SameLine(); // ÇÑ°£ ¶ç±â
		ImGui::Text("test");
	}
};

ATileMapGameMode::ATileMapGameMode()
{
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);
	UEngineGUI::CreateGUIWindow<UTileMapWindow>("TileMapWindow");
}

ATileMapGameMode::~ATileMapGameMode()
{
}

void ATileMapGameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

void ATileMapGameMode::LevelChangeStart()
{
}


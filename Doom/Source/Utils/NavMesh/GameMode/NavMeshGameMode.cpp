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

class UNavMeshDebugWindow : public UEngineGUIWindow
{
public:
	std::shared_ptr<ANavMeshCharacter> TestPlayer;

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

ANavMeshGameMode::ANavMeshGameMode()
{
	{
		GetWorld()->CreateCollisionProfile("Map");
		GetWorld()->CreateCollisionProfile("Player");
	}
	{
		PlayerCharacter = GetWorld()->SpawnActor<ANavMeshCharacter>();
		PlayerCharacter->SetActorLocation(FVector{ 0.0f, 150.0f, 0.0f });
	}
	{
		TestMap = GetWorld()->SpawnActor<ANavMeshMap>();
	}
	{
		Camera = GetWorld()->GetMainCamera();
		Camera->AddActorRelativeLocation({ 0.0f, 80.0f, -500.0f });
		Camera->AddActorRotation({ 10.0f, 0.0f, 0.0f });
		Camera->GetCameraComponent()->SetZSort(0, true);
	}

	std::shared_ptr<UNavMeshDebugWindow> Window = UEngineGUI::CreateGUIWindow<UNavMeshDebugWindow>("NavMeshDebugWindow");
	Window->TestPlayer = PlayerCharacter;

}

ANavMeshGameMode::~ANavMeshGameMode()
{
}

void ANavMeshGameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	DirectX::XMVECTOR OriginVector = DirectX::XMVectorSet(PlayerCharacter->GetActorLocation().X, PlayerCharacter->GetActorLocation().Y, PlayerCharacter->GetActorLocation().Z, 1.0f);

	DirectX::XMVECTOR Direction = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	std::vector<EngineVertex> VertexDataVector = TestMap->GetNavMapComponent()->GetVertexVector();
	std::vector<unsigned int> IndexDataVector = TestMap->GetNavMapComponent()->GetIndexVector();

	for (int i = 0; i < IndexDataVector.size() / 3; i++)
	{

	}

	DirectX::XMVECTOR Vector1 = DirectX::XMVectorSet(-150.0f, 0.0f, 150.0f, 1.0f);
	DirectX::XMVECTOR Vector2 = DirectX::XMVectorSet(150.0f, 0.0f, 150.0f, 1.0f);
	DirectX::XMVECTOR Vector3 = DirectX::XMVectorSet(0.0f, 0.0f, -150.0f, 1.0f);

	DirectX::TriangleTests::Intersects(OriginVector, Direction, Vector1, Vector2, Vector3, DistanceToPlayer);

	if (9.0f < DistanceToPlayer && DistanceToPlayer < 11.0f)
	{
		//SetActorLocation({ 0.0f, 10.0f, 0.0f });
	}
	else
	{
		PlayerCharacter->AddActorLocation({ 0.0f, -0.5f, 0.0f });
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


#include "pch.h"
#include "NavMeshGameMode.h"

#include <Core/Public/Misc/FileHelper.h>
#include <Core/Public/Misc/DirectoryHelper.h>
#include <Core/Public/Serialization/Archive.h>

#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Input/EngineInput.h>
#include <Rendering/Buffer/EngineVertex.h>

#include <Tools/DebugGUI/EngineGUI.h>
#include <Tools/DebugGUI/EngineGUIWindow.h>
#include <ThirdParty/imgui/imgui.h>

#include "Actor/Round1/E1M1Map.h"

#include "Utils/NavMesh/Actor/NavMeshCharacter.h"
#include "Utils/NavMesh/Actor/NavMeshMap.h"
#include "Utils/GUI/GUIEditor.h"
#include "Utils/NavMesh/Component/NavMeshComponent.h"

#include <NavigationSystem/Public/NavigationSystem.h>

class UNavMeshDebugWindow : public UEngineGUIWindow
{
public:
	//std::shared_ptr<ANavMeshCharacter> TestPlayer;


	void SaveAndLoad()
	{

        // Save 버튼 클릭 시
        if (true == ImGui::Button("Save"))
        {
            FDirectoryHelper DirectoryHelper;

            if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
            {
                MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
                return;
            }

            DirectoryHelper.Append("Data");
            std::string InitPath = DirectoryHelper.GetPathToString();

            // 파일 저장 대화상자 설정
            OPENFILENAME ofn = {};  // 대화상자 초기화
            char szFile[260] = { 0 };  // 파일명 버퍼

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = ("All\0*.*\0NaviData\0*.NaviData\0");
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrDefExt = "MapData";
            ofn.lpstrInitialDir = InitPath.c_str();
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // 파일 선택 후 저장
            if (GetSaveFileNameA(&ofn) == TRUE)
            {
                FArchive Ser;
                const auto& NaviDataVector = UNavigationSystem::GetInstance().GetNaviDataVector();

                // NaviDataVector의 크기 저장
                Ser << static_cast<int>(NaviDataVector.size());

                for (const FNaviData& NaviData : NaviDataVector)
                {
                    // IndexArray 저장
                    for (int Index : NaviData.IndexArray)
                    {
                        Ser << Index;
                    }

                    // LinkNaviDataIndex 저장
                    Ser << static_cast<int>(NaviData.LinkNaviDataIndex.size());
                    for (int LinkIndex : NaviData.LinkNaviDataIndex)
                    {
                        Ser << LinkIndex;
                    }

                    // VertexDataVector 저장
                    Ser << static_cast<int>(NaviData.VertexDataVector.size());
                    for (const EngineVertex& Vertex : NaviData.VertexDataVector)
                    {
                        Ser << Vertex.POSITION;
                        Ser << Vertex.TEXCOORD;
                        Ser << Vertex.COLOR;
                    }
                }

                // 파일 열기 및 데이터 쓰기
                FFileHelper NewFile = DirectoryHelper.GetFile(ofn.lpstrFile);

                NewFile.FileOpen("wb");
                NewFile.Write(Ser);
            }
        }
	}

	void OnGUI() override
	{

		ImGui::Text(std::to_string(UNavigationSystem::GetInstance().GetCurNaviDataIndex()).c_str());
		ImGui::Text("Linked Navi Data");
		ImGui::Text(("Distance : " + std::to_string(UNavigationSystem::GetInstance().GetDistance())).c_str());
		for (int i = 0; i < UNavigationSystem::GetInstance().GetPlayerNaviData().LinkNaviDataIndex.size(); i++)
		{
			ImGui::Text(std::to_string(UNavigationSystem::GetInstance().GetPlayerNaviData().LinkNaviDataIndex[i]).c_str());
		}

		std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();

		if (true == ImGui::Button("FreeCameraOn"))
		{
			GetWorld()->GetMainCamera()->SwitchFreeCamera();
		}

		SaveAndLoad();
	}
};

ANavMeshGameMode::ANavMeshGameMode()
{
	{
		GetWorld()->CreateCollisionProfile("Map");
		GetWorld()->CreateCollisionProfile("Player");
	}
	//{
	//	PlayerCharacter = GetWorld()->SpawnActor<ANavMeshCharacter>();
	//	PlayerCharacter->SetActorLocation(FVector{ -150.0f, 100.0f, 50.0f });
	//}
	//{
	//	TestMap = GetWorld()->SpawnActor<AE1M1Map>();
	//}
	//{
	//	Camera = GetWorld()->GetMainCamera();
	//	Camera->AddActorRelativeLocation({ 0.0f, 80.0f, -500.0f });
	//	Camera->AddActorRotation({ 10.0f, 0.0f, 0.0f });
	//	//Camera->AddActorRelativeLocation({ 0.0f, 300.0f, 0.0f });
	//	//Camera->AddActorRotation({ 90.0f, 0.0f, 0.0f });
	//	Camera->GetCameraComponent()->SetZSort(0, true);
	//}

	{
		PlayerCharacter = GetWorld()->SpawnActor<ANavMeshCharacter>();
		PlayerCharacter->SetActorLocation(FVector{ -1042.0f, 14.0f, 3548.0f });
	}
	{
		TestMap = GetWorld()->SpawnActor<AE1M1Map>();
		//TestMap->SetActorRelativeScale3D(FVector{ 1.0f, 1.0f, 1.0f });
		//TestMap->SetActorLocation({ 0.0f, 0.0f, 0.0f });
	}
	{
		NavMap = GetWorld()->SpawnActor<ANavMeshMap>();
		//TestMap->SetActorRelativeScale3D(FVector{ 1.0f, 1.0f, 1.0f });
		//TestMap->SetActorLocation({ 0.0f, 0.0f, 0.0f });
	}
	{
		Camera = GetWorld()->GetMainCamera();
		//Camera->AddActorLocation({ 10.0f, 0.0f, 0.0f });
		Camera->AddActorRelativeLocation({ 0.0f, 10.0f, -100.0f });
		Camera->GetCameraComponent()->SetZSort(0, true);
		Camera->AttachToActor(PlayerCharacter.get());
	}

	std::shared_ptr<UNavMeshDebugWindow> Window = UEngineGUI::CreateGUIWindow<UNavMeshDebugWindow>("NavMeshDebugWindow");
	//Window->TestPlayer = PlayerCharacter;

	UNavigationSystem::GetInstance().Init(PlayerCharacter.get(), NavMap.get(), "");

}

ANavMeshGameMode::~ANavMeshGameMode()
{
}

void ANavMeshGameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	//UNavigationSystem::GetInstance().Tick(DeltaTime);
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


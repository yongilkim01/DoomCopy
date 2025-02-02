#include "Doom.h"
#include "Public/GameModes/E1M1GameMode.h"

#include <Core/Public/Misc/FileHelper.h>
#include <Core/Public/Misc/DirectoryHelper.h>
#include <Core/Public/Serialization/Archive.h>

#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Platform/Public/Input/EngineInput.h>
#include <Rendering/Public/Buffer/EngineVertex.h>

#include <NavigationSystem/Public/NavigationSystem.h>
#include <Editor/Public/EngineGUI.h>
#include <Editor/Public/EngineGUIWindow.h>
#include <ThirdParty/imgui/imgui.h>

#include "Public/Characters/DoomGuy.h"
#include "Public/Characters/Enemy/ImpCharacter.h"
#include "Public/Characters/Enemy/ZombieCharacter.h"
#include "Public/Characters/Enemy/ZombieSgtCharacter.h"

#include "Public/Items/DropItems/DropPotion.h"
#include "Public/Items/DropItems/DropArmor.h"
#include "Public/Items/Environment/DrumContainer.h"

#include "Public/Maps/E1M1Map.h"
#include "Public/Maps/Door.h"

#include "Public/Editor/GUIEditor.h"
#include "Public/NavigationSystem/NavMeshMap.h"


class E1M1DebugWindow : public UEngineGUIWindow
{
public:
	std::shared_ptr<ADoomGuyCharacter> DoomGuy;

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

		ImGui::Text("Player : %s", GetWorld()->GetMainPawn()->GetActorLocation().ToString().c_str());
	}
};

AE1M1GameMode::AE1M1GameMode()
{
	{
		GetWorld()->CreateCollisionProfile("PlayerBody");
		GetWorld()->CreateCollisionProfile("PlayerAttack");
		GetWorld()->CreateCollisionProfile("EnemyBody");
		GetWorld()->CreateCollisionProfile("EnemyAttack");
		GetWorld()->CreateCollisionProfile("DropItem");
		GetWorld()->CreateCollisionProfile("EnvBody");
		GetWorld()->CreateCollisionProfile("EnvAttack");

		GetWorld()->LinkCollisionProfile("PlayerAttack", "EnemyBody");
		GetWorld()->LinkCollisionProfile("EnemyAttack", "PlayerBody");
		GetWorld()->LinkCollisionProfile("DropItem", "PlayerBody");
		GetWorld()->LinkCollisionProfile("PlayerAttack", "EnvBody");
		GetWorld()->LinkCollisionProfile("EnvAttack", "PlayerBody");
	}
	{
		E1M1Map = GetWorld()->SpawnActor<AE1M1Map>();
		NavMap = GetWorld()->SpawnActor<ANavMeshMap>();
	}
	{
		std::shared_ptr<ADoor> Door1 = GetWorld()->SpawnActor<ADoor>();
		Door1->SetModel("doom_E1M1", 15);

		std::shared_ptr<ADoor> Door2 = GetWorld()->SpawnActor<ADoor>();
		Door2->SetModel("doom_E1M1", 32);

		std::shared_ptr<ADoor> Door3 = GetWorld()->SpawnActor<ADoor>();
		Door3->SetModel("doom_E1M1", 34);
	}
	{
		std::shared_ptr<AZombieSgtCharacter> ZombieSgtCharacter = GetWorld()->SpawnActor<AZombieSgtCharacter>();
		ZombieSgtCharacter->SetActorLocation(FVector{ 30.0f, 130.0f, 3179.0f });

		ZombieSgtCharacter->AddTurningLocation(FVector{ 30.0f, 130.0f, 3179.0f });
		ZombieSgtCharacter->AddTurningLocation(FVector{ 30.0f, 130.0f, 3300.0f });
		ZombieSgtCharacter->AddTurningLocation(FVector{ 130.0f, 130.0f, 3300.0f });
		ZombieSgtCharacter->AddTurningLocation(FVector{ 130.0f, 130.0f, 3179.0f });

		std::shared_ptr<AZombieSgtCharacter> ZombieSgtCharacter2 = GetWorld()->SpawnActor<AZombieSgtCharacter>();
		ZombieSgtCharacter2->SetActorLocation(FVector{ 130.0f, 130.0f, 3300.0f });

		ZombieSgtCharacter2->AddTurningLocation(FVector{ 30.0f, 130.0f, 3300.0f });
		ZombieSgtCharacter2->AddTurningLocation(FVector{ 130.0f, 130.0f, 3300.0f });
		ZombieSgtCharacter2->AddTurningLocation(FVector{ 130.0f, 130.0f, 3179.0f });

		std::shared_ptr<AZombieSgtCharacter> ZombieSgtCharacter3 = GetWorld()->SpawnActor<AZombieSgtCharacter>();
		ZombieSgtCharacter3->SetActorLocation(FVector{ -2252, 40, 2503 });

		ZombieSgtCharacter3->AddTurningLocation(FVector{ -2252, 40, 2503 });
		ZombieSgtCharacter3->AddTurningLocation(FVector{ -2100, 30, 2503 });
		ZombieSgtCharacter3->AddTurningLocation(FVector{ -2100, 30, 2362 });
		ZombieSgtCharacter3->AddTurningLocation(FVector{ -2252, 40, 2362 });

		std::shared_ptr<AZombieSgtCharacter> ZombieSgtCharacter4 = GetWorld()->SpawnActor<AZombieSgtCharacter>();
		ZombieSgtCharacter4->SetActorLocation(FVector{ -2417, 10, 2516 });

		ZombieSgtCharacter4->AddTurningLocation(FVector{ -2417, 10, 2322 });
		ZombieSgtCharacter4->AddTurningLocation(FVector{ -2417, 10, 2516 });

		std::shared_ptr<AZombieSgtCharacter> ZombieSgtCharacter5 = GetWorld()->SpawnActor<AZombieSgtCharacter>();
		ZombieSgtCharacter5->SetActorLocation(FVector{ -140, 5, 3386 });

		ZombieSgtCharacter5->AddTurningLocation(FVector{ -140, 5, 3386 });
		ZombieSgtCharacter5->AddTurningLocation(FVector{ -185, 10, 3331 });
		ZombieSgtCharacter5->SetDropItemType(EDropItemType::SHOTGUN);

	}
	{
		// -2252, 35, 2503
		// -2252, 35, 2362
	}
	{
		std::shared_ptr<AZombieCharacter> ZombieCharacter1 = GetWorld()->SpawnActor<AZombieCharacter>();
		ZombieCharacter1->SetActorLocation(FVector{ -2252, 40, 2362 });

		ZombieCharacter1->AddTurningLocation(FVector{ -2252, 40, 2362 });
		ZombieCharacter1->AddTurningLocation(FVector{ -2252, 40, 2503 });
		ZombieCharacter1->AddTurningLocation(FVector{ -2100, 30, 2503 });
		ZombieCharacter1->AddTurningLocation(FVector{ -2100, 30, 2362 });

		std::shared_ptr<AZombieCharacter> ZombieCharacter2 = GetWorld()->SpawnActor<AZombieCharacter>();
		ZombieCharacter2->SetActorLocation(FVector{ -2232, 40, 2402 });

		ZombieCharacter2->AddTurningLocation(FVector{ -2232, 40, 2402 });
		ZombieCharacter2->AddTurningLocation(FVector{ -2182, 35, 2402 });
		ZombieCharacter2->AddTurningLocation(FVector{ -2182, 35, 2480 });
		ZombieCharacter2->AddTurningLocation(FVector{ -2232, 40, 2480 });

		std::shared_ptr<AZombieCharacter> ZombieCharacter3 = GetWorld()->SpawnActor<AZombieCharacter>();
		ZombieCharacter3->SetActorLocation(FVector{ -2978, 20, 2938 });

		ZombieCharacter3->AddTurningLocation(FVector{ -2978, 20, 2938 });
		ZombieCharacter3->AddTurningLocation(FVector{ -2869, 20, 2757 });
		//ZombieCharacter3->SetDetectRange(100.0f);

		//std::shared_ptr<AImpCharacter> ImpCharacter = GetWorld()->SpawnActor<AImpCharacter>();
		//ImpCharacter->SetActorLocation(FVector{ -1092.0f, 27.0f, 3555.0f });
	}
	{
		std::shared_ptr<ADropPotion> Potion1 = GetWorld()->SpawnActor<ADropPotion>();
		Potion1->SetActorLocation(FVector{ -741, 10, 3524 });
		// -752, 10, 3520
	}
	{
		std::shared_ptr<ADropArmor> Armor = GetWorld()->SpawnActor<ADropArmor>();
		Armor->SetActorLocation(FVector{ 221, 138, 3227 });
		// -752, 10, 3520
	}
	{
		std::shared_ptr<ADrumContainer> DrumContainer1 = GetWorld()->SpawnActor<ADrumContainer>();
		DrumContainer1->SetActorLocation(FVector{ -1116, 17, 2932 });
	}
	//{
	//	Camera = GetWorld()->GetMainCamera();
	//	Camera->AddActorRelativeLocation({ 0.0f, 30.0f, -10.0f });
	//	Camera->GetCameraComponent()->SetZSort(0, true);
	//	Camera->AttachToActor(GetWorld()->GetMainPawn());
	//}

	std::shared_ptr<E1M1DebugWindow> Window = UEngineGUI::CreateGUIWindow<E1M1DebugWindow>("E1M1DebugWindow");
}

AE1M1GameMode::~AE1M1GameMode()
{
}

void AE1M1GameMode::BeginPlay()
{
	AGameMode::BeginPlay();

	UNavigationSystem::GetInstance().Init(GetWorld()->GetMainPawn(), NavMap.get(), "");
}

void AE1M1GameMode::Tick(float DeltaTime)
{
	AGameMode::Tick(DeltaTime);

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

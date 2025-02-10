#include "Doom.h"
#include "Public/Maps/E1M1Map.h"

#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>
#include <Core/Public/Misc/DirectoryHelper.h>

AE1M1Map::AE1M1Map()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	FDirectoryHelper DirectoryHelper;

	if (false == DirectoryHelper.MoveParentToDirectory("Resources", "Doom"))
	{
		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
		return;
	}

	DirectoryHelper.Append("Models\\E1M1");

	std::string Path = DirectoryHelper.GetPathToString();

	{
		GroundMeshComponent = CreateDefaultSubObject<UStaticMeshComponent>();
		GroundMeshComponent->SetupAttachment(RootComponent);

		std::vector<int> GroundModelNumber;
		GroundModelNumber.reserve(20);

		GroundModelNumber.push_back(35);
		GroundModelNumber.push_back(36);
		GroundModelNumber.push_back(37);
		GroundModelNumber.push_back(41);
		GroundModelNumber.push_back(45);
		GroundModelNumber.push_back(51);
		GroundModelNumber.push_back(52);

		GroundMeshComponent->SetModel("doom_E1M1", GroundModelNumber);
	}
	//{
	//	DoorMeshComponent = CreateDefaultSubObject<UStaticMeshComponent>();
	//	DoorMeshComponent->SetupAttachment(RootComponent);

	//	std::vector<int> DoorModelNumber;
	//	DoorModelNumber.reserve(5);

	//	DoorModelNumber.push_back(15);
	//	DoorModelNumber.push_back(32);
	//	DoorModelNumber.push_back(34);

	//	DoorMeshComponent->SetModel("doom_E1M1", DoorModelNumber);
	//}
	{
		WallMeshComponent = CreateDefaultSubObject<UStaticMeshComponent>();
		WallMeshComponent->SetupAttachment(RootComponent);

		std::vector<int> WallModelNumber;
		WallModelNumber.reserve(60);

		WallModelNumber.push_back(1);
		WallModelNumber.push_back(2);
		WallModelNumber.push_back(3);
		WallModelNumber.push_back(4);
		WallModelNumber.push_back(5);
		WallModelNumber.push_back(6);
		WallModelNumber.push_back(7);
		WallModelNumber.push_back(8);
		WallModelNumber.push_back(9);
		WallModelNumber.push_back(10);
		WallModelNumber.push_back(11);
		WallModelNumber.push_back(12);
		WallModelNumber.push_back(13);
		WallModelNumber.push_back(14);	
		WallModelNumber.push_back(16);	
		WallModelNumber.push_back(17);
		WallModelNumber.push_back(18);
		WallModelNumber.push_back(19);
		WallModelNumber.push_back(20);
		WallModelNumber.push_back(21);
		WallModelNumber.push_back(22);
		WallModelNumber.push_back(23);
		WallModelNumber.push_back(24);
		WallModelNumber.push_back(25);
		WallModelNumber.push_back(26);
		WallModelNumber.push_back(27);
		WallModelNumber.push_back(28);
		WallModelNumber.push_back(29);
		WallModelNumber.push_back(30);
		WallModelNumber.push_back(31);
		WallModelNumber.push_back(33);
		WallModelNumber.push_back(38);
		WallModelNumber.push_back(39);
		//WallModelNumber.push_back(40);
		WallModelNumber.push_back(42);
		WallModelNumber.push_back(43);
		WallModelNumber.push_back(44);
		WallModelNumber.push_back(46);
		WallModelNumber.push_back(47);
		WallModelNumber.push_back(48);
		WallModelNumber.push_back(49);
		WallModelNumber.push_back(50);
		WallModelNumber.push_back(53);
		WallModelNumber.push_back(54);
		WallModelNumber.push_back(55);
		WallModelNumber.push_back(56);

		WallMeshComponent->SetModel("doom_E1M1", WallModelNumber);
	}
	// 56
}

AE1M1Map::~AE1M1Map()
{
}

void AE1M1Map::BeginPlay()
{
	AActor::BeginPlay();
}

void AE1M1Map::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

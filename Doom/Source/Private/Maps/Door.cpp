#include "Doom.h"
#include "Public/Maps/Door.h"

#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>
#include <Core/Public/Misc/DirectoryHelper.h>

ADoor::ADoor()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	DoorMeshComponent = CreateDefaultSubObject<UStaticMeshComponent>();
	DoorMeshComponent->SetupAttachment(RootComponent);

}

ADoor::~ADoor()
{
}

// 15, 32, 34
void ADoor::SetModel(std::string_view ModelName, int ModelNumber)
{
	FDirectoryHelper DirectoryHelper;

	if (false == DirectoryHelper.MoveParentToDirectory("Resources", "Doom"))
	{
		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
		return;
	}

	DirectoryHelper.Append("Models\\E1M1");

	std::string Path = DirectoryHelper.GetPathToString();

	{
		DoorMeshComponent = CreateDefaultSubObject<UStaticMeshComponent>();
		DoorMeshComponent->SetupAttachment(RootComponent);

		std::vector<int> DoorModelNumber;
		DoorModelNumber.reserve(1);
		DoorModelNumber.push_back(ModelNumber);

		DoorMeshComponent->SetModel(ModelName, DoorModelNumber);
	}
}

void ADoor::BeginPlay()
{
	AActor::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

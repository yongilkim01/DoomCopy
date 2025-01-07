#include "pch.h"
#include "E1M1Map.h"

#include <Classes/Components/StaticMeshComponent.h>
#include <Classes/Components/ShapeComponent.h>
#include <Core/Misc/DirectoryHelper.h>

AE1M1Map::AE1M1Map()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	FDirectoryHelper DirectoryHelper;

	if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
		return;
	}

	DirectoryHelper.Append("Models\\E1M1");

	std::string Path = DirectoryHelper.GetPathToString();

	MeshComponent = CreateDefaultSubObject<UStaticMeshComponent>();
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetModel("doom_E1M1");
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

#include "pch.h"
#include "DrawLineActor.h"

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/SceneComponent.h>
#include <Classes/Components/StaticMeshComponent.h>
#include <Core/Misc/DirectoryHelper.h>
#include <Input/EngineInput.h>

#include "Componennts/LineComponent.h"
#include "Utils/Wad.h"

ADrawLineActor::ADrawLineActor()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	FDirectoryHelper DirectoryHelper;

	if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("���ҽ� ������ ã�⿡ �����߽��ϴ�");
		return;
	}

	DirectoryHelper.Append("Models\\E1M1");

	std::string Path = DirectoryHelper.GetPathToString();

	LineComponent = CreateDefaultSubObject<UStaticMeshComponent>();
	LineComponent->SetupAttachment(RootComponent);
	LineComponent->SetModel("doom_E1M1");
	//LineComponent->InitObjFile(Path ,"E1M1", Path + "\\doom_E1M1.obj", Path + "\\doom_E1M1.mtl");
	//
	// LineComponent->SetRelativeScale3D({1.0f, 1.0f, 1.0f});
}

ADrawLineActor::~ADrawLineActor()
{
}

void ADrawLineActor::BeginPlay()
{
	AActor::BeginPlay();
}

void ADrawLineActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

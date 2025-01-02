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
		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
		return;
	}

	DirectoryHelper.Append("Models\\E1M1");

	std::string Path = DirectoryHelper.GetPathToString();

	LineComponent = CreateDefaultSubObject<UStaticMeshComponent>();
	LineComponent->Init(Path + "\\doom_E1M1.obj", Path + "\\doom_E1M1.mtl");
	LineComponent->SetupAttachment(RootComponent);
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

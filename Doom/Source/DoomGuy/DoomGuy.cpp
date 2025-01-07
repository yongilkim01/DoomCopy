#include "pch.h"
#include "DoomGuy.h"

#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/StaticMeshComponent.h>
#include <Classes/Components/ShapeComponent.h>
#include <Classes/Camera/CameraActor.h>
#include <Classes/Camera/CameraComponent.h>

#include <Core/Misc/DirectoryHelper.h>

#include <Input/EngineInput.h>

ADoomGuy::ADoomGuy()
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

ADoomGuy::~ADoomGuy()
{
}

void ADoomGuy::BeginPlay()
{
	AActor::BeginPlay();

	Camera = GetWorld()->GetMainCamera();

	PrevMouseLocation = Camera->ScreenMouseLocationToWorldLocation();
}

void ADoomGuy::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	//FVector CurMouseLocation = Camera->ScreenMouseLocationToWorldLocation();

	//UEngineDebug::OutPutString(Camera->GetMouseLocation().ToString());

	//if (UEngineInput::IsPress('A'))
	//{
	//	AddActorLocation(FVector{ -200.0f * DeltaTime, 0.0f, 0.0f });
	//}
	//if (UEngineInput::IsPress('D'))
	//{
	//	AddActorLocation(FVector{ 200.0f * DeltaTime, 0.0f, 0.0f });
	//}
	//if (UEngineInput::IsPress('W'))
	//{
	//	AddActorLocation(FVector{ 0.0f, 0.0f, 200.0f * DeltaTime, 0.0f });
	//}
	//if (UEngineInput::IsPress('S'))
	//{
	//	AddActorLocation(FVector{ 0.0f, 0.0f, -200.0f * DeltaTime, 0.0f });
	//}
}

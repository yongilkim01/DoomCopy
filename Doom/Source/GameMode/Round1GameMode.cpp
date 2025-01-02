#include "pch.h"
#include "Round1GameMode.h"

#include "Actor/Round1/DoomMap.h"

#include "Utils/DrawLineActor.h"
#include "Utils/DrawSquareActor.h"

#include <Classes/Camera/CameraActor.h>
#include <Input/EngineInput.h>


ARound1GameMode::ARound1GameMode()
{
	//{
	//	std::shared_ptr<ADrawSquareActor> DrawSquareActor = GetWorld()->SpawnActor<ADrawSquareActor>();
	//	DrawSquareActor->SetActorRelativeScale3D(FVector{ 1.0f, 1.0f, 1.0f });
	//	DrawSquareActorVector.push_back(DrawSquareActor);
	//}

	{
		std::shared_ptr<ADrawLineActor> DrawLineActor = GetWorld()->SpawnActor<ADrawLineActor>();
		//DrawLineActor->SetActorRotation(FVector{ 0.0f, 0.0f, 30.0f });
		DrawLineActor->SetActorRelativeScale3D(FVector{ 1.0f, 1.0f, 1.0f });
		DrawLineActor->SetActorRelativeScale3D({ 2.f, 2.f, 2.f });
		DrawLineActor->SetActorLocation({ 60.0f, 0.f, 0.f });
	}

	Camera = GetWorld()->GetMainCamera();
	Camera->AddActorLocation({ 10.0f, 0.0f, 0.0f });
	Camera->AddActorRelativeLocation({ 0.0f, 0.0f, -500.0f});
}

ARound1GameMode::~ARound1GameMode()
{
}

void ARound1GameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	//UEngineDebug::OutPutString(Camera->ScreenMouseLocationToWorldLocation().ToString());


	if (UEngineInput::IsPress('A'))
	{
		Camera->AddActorLocation(FVector{ -2000.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		Camera->AddActorLocation(FVector{ 2000.0f * DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('W'))
	{
		Camera->AddActorLocation(FVector{ 0.0f, 0.0f, 2000.0f * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		Camera->AddActorLocation(FVector{ 0.0f, 0.0f, -2000.0f * DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress(VK_SPACE))
	{
		Camera->AddActorLocation(FVector{ 0.0f, 2000.0f * DeltaTime, 0.0f, 0.0f });
	}
}
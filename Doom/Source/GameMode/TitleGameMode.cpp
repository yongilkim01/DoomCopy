#include "pch.h"
#include "TitleGameMode.h"
#include "Actor/TitleLogo.h"

#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Components/PaperSpriteComponent.h>

ATitleGameMode::ATitleGameMode()
{
	{
		TitleLogo = GetWorld()->SpawnActor<ATitleLogo>();
		// Logo->SetActorLocation({ 300.0f, 0.0f, 0.0f });
	}

	// ī�޶� �����Ÿ� �ڷ� ���� 
	// ī�޶� ��ġ������ ������ ����� �Ұ��̴�.
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}

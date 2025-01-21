#include "pch.h"
#include "Public/Items/Weapons/BaseWeapon.h"

#include <Engine/Classes/Components/PaperSpriteComponent.h>

ABaseWeapon::ABaseWeapon()
{
	std::shared_ptr<USceneComponent> SceneComponent = CreateDefaultSubObject<USceneComponent>();
	RootComponent = SceneComponent;

	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetTexture("Shotgun.png");
	SpriteComponent->SetRelativeLocation({ 0.0f, 24.5f, 5.0f });
	SpriteComponent->SetWorldScale3D({ 5.0f, 5.0f });
}

ABaseWeapon::~ABaseWeapon()
{

}

void ABaseWeapon::BeginPlay()
{
	AActor::BeginPlay();
}

void ABaseWeapon::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);

    UEngineDebug::OutPutString("Weapon Location : " + GetActorLocation().ToString());


    if (true == bMoving)
    {
        // �ð� �� ����
        RunningTime += DeltaTime;

        // ������ ���
        float Amplitude = 0.005f;  // ����
        float Frequency = 1.0f;    // ���ļ�
        float DeltaHeight = Amplitude * FMath::Sin(2 * FMath::PI * Frequency * RunningTime);
        float DeltaWidth = Amplitude * FMath::Sin(2 * FMath::PI * Frequency * RunningTime * 2.0f);

        //UEngineDebug::OutPutString("Sin Value : " + std::to_string(DeltaHeight));

        // ���� ��ġ�� ���ϱ�
        FVector NewLocation = GetActorLocation();

        NewLocation.Y += DeltaHeight;

        //SetActorLocation(NewLocation);
        AddActorLocation(FVector{DeltaHeight,- DeltaWidth, 0.0f });
    }
}

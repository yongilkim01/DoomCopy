#include "pch.h"
#include "Engine/Classes/Camera/CameraActor.h"

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Engine/GameEngine.h"
#include "Platform/Window.h"
#include "Input/EngineInput.h"

ACameraActor::ACameraActor()
{
	CameraComponent = CreateDefaultSubObject<UCameraComponent>();
	RootComponent = CameraComponent;
}

ACameraActor::~ACameraActor()
{
	CameraComponent = nullptr;
}


void ACameraActor::BeginPlay()
{
	AActor::BeginPlay();
}

void ACameraActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	ScreenPos = UGameEngine::GetMainWindow().GetMousePos();
	FVector ScreenDir = PrevScreenPos - ScreenPos;
	ScreenDir.Normalize();
	PrevScreenPos = ScreenPos;

	if (true == bFreeCameraValue)
	{
		if (UEngineInput::IsDown('O'))
		{
			switch (CameraComponent->ProjectionType)
			{
			case EProjectionType::Perspective:
				CameraComponent->ProjectionType = EProjectionType::Orthographic;
				break;
			case EProjectionType::Orthographic:
				CameraComponent->ProjectionType = EProjectionType::Perspective;
				break;
			default:
				break;
			}
		}
		float Speed = FreeSpeed;
		if (UEngineInput::IsPress(VK_LSHIFT))
		{
			Speed *= 5.0f;
		}
		if (UEngineInput::IsPress('A'))
		{
			AddActorLocation(-GetActorTransform().GetWorldRight() * DeltaTime * Speed);
		}
		if (UEngineInput::IsPress('D'))
		{
			AddActorLocation(GetActorTransform().GetWorldRight() * DeltaTime * Speed);
		}
		if (UEngineInput::IsPress('W'))
		{
			AddActorLocation(GetActorTransform().GetWorldFoward() * DeltaTime * Speed);
		}
		if (UEngineInput::IsPress('S'))
		{
			AddActorLocation(-GetActorTransform().GetWorldFoward() * DeltaTime * Speed);
		}
		if (UEngineInput::IsPress('Q'))
		{
			AddActorLocation(GetActorTransform().GetWorldUp() * DeltaTime * Speed);
		}
		if (UEngineInput::IsPress('E'))
		{
			AddActorLocation(-GetActorTransform().GetWorldUp() * DeltaTime * Speed);
		}
		if (UEngineInput::IsPress(VK_RBUTTON))
		{
			AddActorRotation({ -ScreenDir.Y * RotSpeed * DeltaTime, -ScreenDir.X * RotSpeed * DeltaTime });
		}
	}

	CameraComponent->CalculateViewAndProjection();

}

FVector ACameraActor::ScreenMouseLocationToWorldLocation()
{
	FVector Size = UGameEngine::GetMainWindow().GetWindowSize();

	FVector MousePos = UGameEngine::GetMainWindow().GetMousePos();

	float4x4 Mat;

	Mat.ViewPort(Size.X, Size.Y, 0.0f, 0.0f, 0.0f, 1.0f);

	FTransform CameraTransform = GetActorTransform();

	MousePos = MousePos * Mat.InverseReturn();
	MousePos = MousePos * CameraTransform.Projection.InverseReturn();
	MousePos = MousePos * CameraTransform.View.InverseReturn();

	return MousePos;
}

FVector ACameraActor::ScreenMouseLocationToWorldLocationWithOutLocation()
{
	return FVector();
}

FVector ACameraActor::GetMouseLocation()
{
	FTransform RendererTransform = GetActorTransform();

	RendererTransform.WVP = RendererTransform.World * RendererTransform.View * RendererTransform.Projection;

	return RendererTransform.WorldLocation;
}

void ACameraActor::OnFreeCamera()
{
	bFreeCameraValue = true;
	CheckCameraCheck();
}

void ACameraActor::OffFreeCamera()
{
	bFreeCameraValue = false;
}

void ACameraActor::SwitchFreeCamera()
{
	bFreeCameraValue = !bFreeCameraValue;
	CheckCameraCheck();
}

void ACameraActor::CheckCameraCheck()
{
	if (true == bFreeCameraValue)
	{
		PrevTrans = GetActorTransform();
		PrevProjectionType = GetCameraComponent()->ProjectionType;
		GetCameraComponent()->ProjectionType = EProjectionType::Perspective;
	}
	else
	{
		SetActorTransform(PrevTrans);
		GetCameraComponent()->ProjectionType = PrevProjectionType;
	}
}

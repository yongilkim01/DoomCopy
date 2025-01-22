#pragma once

#include "Engine/Classes/Engine/Object.h"
#include "Core/Public/Debugging/DebugMacros.h"
#include "Engine/Classes/Components/LightComponent.h"

class UShapeComponent;
class UPrimitiveComponent;
class AActor;
class ACameraActor;
class UGameEngine;
class AGameMode;
class APawn;
class URenderTarget;
class AHUD;
class UWidget;
class ALight;

/**
 *	엔진 레벨 클래스
 */
class ULevel : public UObject
{
	friend UShapeComponent;
	friend UGameEngine;
	friend ULightComponent;
public:
	/** 생성자, 소멸자 */
	ENGINE_API ULevel();
	ENGINE_API ~ULevel();

	/** 객체 값 복사 방지 */
	ULevel(const ULevel& Other) = delete;
	ULevel(ULevel&& Other) noexcept = delete;
	ULevel& operator=(const ULevel& Other) = delete;
	ULevel& operator=(ULevel&& Other) noexcept = delete;

	void LevelChangeStart();
	void LevelChangeEnd();

	void Tick(float DeltaTime);
	void Render(float DeltaTime);
	void Collision(float DeltaTime);
	void Release(float DeltaTime);

	ENGINE_API void ChangeRenderGroup(int CameraOrder, int PrevGroupOrder, std::shared_ptr<UPrimitiveComponent> Renderer);
	ENGINE_API void CreateCollisionProfile(std::string_view ProfileName);
	ENGINE_API void PushCollisionProfileName(std::shared_ptr<UPrimitiveComponent> PrComp);
	ENGINE_API void LinkCollisionProfile(std::string_view LeftProfileName, std::string_view RightProfileName);
	ENGINE_API void ChangeCollisionProfileName(std::string_view ProfileName, std::string_view PrevProfileName, std::shared_ptr<UShapeComponent> ShapeComponent);
	ENGINE_API void PushLight(std::shared_ptr<ULightComponent> LightComponent);
	
	template<typename ActorType>
	std::shared_ptr<ActorType> SpawnActor(std::string_view ActorName = "")
	{
		static_assert(std::is_base_of_v<AActor, ActorType>, "액터를 상속받지 않은 클래스");

		if (false == std::is_base_of_v<AActor, ActorType>)
		{
			MSGASSERT("액터를 상속받지 않은 클래스를 SpawnActor 시도");
			return nullptr;
		}

		char* ActorMemory = new char[sizeof(ActorType)];

		AActor* PtrActor = reinterpret_cast<ActorType*>(ActorMemory);
		PtrActor->World = this;

		ActorType* NewPtr = reinterpret_cast<ActorType*>(ActorMemory);
		std::shared_ptr<ActorType> NewActor(NewPtr = new(ActorMemory) ActorType());

		PtrActor->SetName(ActorName);

		BeginPlayList.push_back(NewActor);

		return NewActor;
	}

	template<typename EnumType>
	std::shared_ptr<class ACameraActor> SpawnCamera(EnumType Order)
	{
		return SpawnCamera(static_cast<int>(Order));
	}
	std::shared_ptr<class ACameraActor> SpawnCamera(int Order);

	template<typename ConvertType>
	ENGINE_API std::list<std::shared_ptr<ConvertType>> GetAllActorListByClass()
	{
		std::list<std::shared_ptr<ConvertType>> List;
		for (std::shared_ptr<class AActor> Actor : AllActorList)
		{
			std::shared_ptr<ConvertType> Convert = std::dynamic_pointer_cast<ConvertType>(Actor);
			if (nullptr == Convert)
			{
				continue;
			}
			List.push_back(Convert);
		}
		return List;
	}
	template<typename ConvertType>
	ENGINE_API std::vector<std::shared_ptr<ConvertType>> GetAllActorArrayByClass()
	{
		std::vector<std::shared_ptr<ConvertType>> List;
		for (std::shared_ptr<class AActor> Actor : AllActorList)
		{
			std::shared_ptr<ConvertType> Convert = std::dynamic_pointer_cast<ConvertType>(Actor);
			if (nullptr == Convert)
			{
				continue;
			}
			List.push_back(Convert);
		}
		return List;
	}
	/** 겟, 셋 메소드 */
	std::shared_ptr<ACameraActor> GetMainCamera()
	{
		return GetCamera(0);
	}
	template<typename EnumType>
	std::shared_ptr<class ACameraActor> GetCamera(EnumType CameraOrder)
	{
		return GetCamera(static_cast<int>(CameraOrder));
	}
	std::shared_ptr<ACameraActor> GetCamera(int CameraOrder)
	{
		if (false == Cameraes.contains(CameraOrder))
		{
			MSGASSERT("존재하지 않는 카메라입니다.");
		}
		return Cameraes[CameraOrder];
	}
	template<typename GameModeType>
	GameModeType* GetGameMode()
	{
		return dynamic_cast<GameModeType*>(GameMode);
	}
	AGameMode* GetGameMode()
	{
		return GameMode;
	}
	template<typename PawnType>
	PawnType* GetMainPawn()
	{
		return dynamic_cast<PawnType*>(MainPawn);
	}
	APawn* GetMainPawn()
	{
		return MainPawn;
	}
	template<typename HUDType>
	HUDType* GetHUD()
	{
		return dynamic_cast<HUDType>(HUD);
	}
	AHUD* GetHUD()
	{
		return HUD;
	}

protected:

private:
	ENGINE_API void InitLevel(AGameMode* InitGameMode, APawn* InitPawn, AHUD* HUD);

	std::list<std::shared_ptr<AActor>> BeginPlayList;
	std::list<std::shared_ptr<AActor>> AllActorList;
	std::map<int, std::list<std::shared_ptr<UPrimitiveComponent>>> RendererMap;
	std::map<int, std::shared_ptr<ACameraActor>> Cameraes;
	std::map<std::string, std::list<std::shared_ptr<UShapeComponent>>> ShapeCompMap;
	std::map<std::string, std::list<std::shared_ptr<UShapeComponent>>> CheckShapeCompMap;
	std::map<std::string, std::list<std::string>> CollisionLinkMap;
	std::vector<std::shared_ptr<ULightComponent>> LightComponentVector;
	FLightDatas LightDatas;

	std::shared_ptr<URenderTarget> FinalRenderTarget;

	AGameMode* GameMode = nullptr;
	APawn* MainPawn = nullptr;
	AHUD* HUD = nullptr;
};
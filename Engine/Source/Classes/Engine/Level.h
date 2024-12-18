#pragma once

#include "Core/Object/Object.h"
#include "WorldPartition/DebugHelpers.h"
//#include "GameFramework/Actor.h"

class URenderer;
class AActor;

/**
 *	엔진 레벨 클래스
 */
class ULevel : public UObject
{
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

	void ChangeRenderGroup(int PrevGroupOrder, std::shared_ptr<URenderer> Renderer);

	template<typename ActorType>
	std::shared_ptr<ActorType> SpawnActor()
	{
		// 템플릿으로 전달받은 액터 타입이 액터를 상속받지 않았을 경우 체크
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

		BeginPlayList.push_back(NewActor);

		return NewActor;
	}

protected:

private:
	std::list<std::shared_ptr<AActor>> BeginPlayList;
	std::list<std::shared_ptr<AActor>> AllActorList;
	std::map<int, std::list<std::shared_ptr<URenderer>>> RendererMap;

};
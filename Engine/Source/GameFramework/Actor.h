#pragma once
#include "Core/Object/Object.h"
#include "WorldPartition/DebugHelpers.h"
#include "EngineDefine.h"

class USceneComponent;
class UActorComponent;

/**
 *	액터 정의 클래스 
 */
class AActor : public UObject
{
	friend class ULevel;
public:
	/** 생성자, 소멸자 */
	ENGINE_API AActor();
	ENGINE_API ~AActor();

	/** 객체 값 복사 방지 */
	AActor(const AActor& Other) = delete;
	AActor(AActor&& Other) noexcept = delete;
	AActor& operator=(const AActor& Other) = delete;
	AActor& operator=(AActor&& Other) noexcept = delete;

	ENGINE_API virtual void BeginPlay();
	ENGINE_API virtual void Tick(float DeltaTime);

	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}

	template<typename ComponentType>
	void CreateDefaultSubObject()
	{
		static_assert(std::is_base_of_v<UActorComponent, ComponentType>, "액터 컴포넌트를 상속받지 않은 클래스를 CreateDefaultSubObject하려고 시도");

		if (false == std::is_base_of_v<UActorComponent, ComponentType>)
		{
			MSGASSERT("ActorComponent를 상속받지 않은 클래스를 CreateDefaultSubObject 시도");
			return nullptr;
		}

		char* ComponentMemory = new char[sizeof(ComponentType)];

		UActorComponent* ComponentPtr = reinterpret_cast<ComponentType*>(ComponentMemory);
		PtrComponent->SetOwner(this);

		ComponentType* NewPtr = reinterpret_cast<ComponentType*>(ComponentMemory);

		std::shared_ptr<ComponentType> NewComponent(new(ComponentMemory)ComponentType());

		if (std::is_base_of_v<USceneComponent, ComponentType>)
		{
			if (nullptr != RootComponent)
			{
				MSGASSERT("아직 기하구조를 만들지 않았습니다.");
			}

			RootComponent = NewComponent;
		}
		else if (std::is_base_of_v<UActorComponent, ComponentType>)
		{

		}
		else
		{
			MSGASSERT("말도 안됨");
		}

		return NewComponent;
	}

	ULevel* GetWorld()
	{
		return World;
	}

protected:

private:
	ULevel* World;

	std::shared_ptr<USceneComponent> RootComponent = nullptr;
	std::list<std::shared_ptr<UActorComponent>> ActorComponentList;

};
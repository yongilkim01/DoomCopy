#pragma once
#include "Core/Object/Object.h"
#include "WorldPartition/DebugHelpers.h"
#include "EngineDefine.h"
#include "Classes/Components/SceneComponent.h"

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
	inline std::shared_ptr<ComponentType> CreateDefaultSubObject()
	{
		/** 액터 컴포넌트를 상속받았는지 검사 */
		static_assert(std::is_base_of_v<UActorComponent, ComponentType>, 
			"액터 컴포넌트를 상속받지 않은 클래스를 CreateDefaultSubObject하려고 시도");
		if (false == std::is_base_of_v<UActorComponent, ComponentType>)
		{
			MSGASSERT("액터 컴포넌트를 상속받지 않은 클래스를 CreateDefaultSubObject하려고 시도");
			return nullptr;
		}

		char* ComponentMemory = new char[sizeof(ComponentType)];

		UActorComponent* ComponentPtr = reinterpret_cast<ComponentType*>(ComponentMemory);
		ComponentPtr->SetOwner(this);

		ComponentType* NewPtr = reinterpret_cast<ComponentType*>(ComponentMemory);

		std::shared_ptr<ComponentType> NewComponent(new(ComponentMemory)ComponentType());

		/** 컴포넌트가 SceneComponent일 경우 */
		if (std::is_base_of_v<USceneComponent, ComponentType>)
		{
			if (nullptr != RootComponent)
			{
				MSGASSERT("Root component가 할당되지 않았습니다.");
			}

			RootComponent = NewComponent;
		}
		else if (std::is_base_of_v<UActorComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewComponent);
		}
		else
		{
			MSGASSERT("어떤 컴포넌트도 아닙니다.");
		}

		return NewComponent;
	}

	void SetActorLocation(const FVector& NewLocation)
	{
		if (nullptr == RootComponent)
		{
			return;
		}
		RootComponent->SetWorldLocation(NewLocation);
	}

	void SetActorRelativeScale3D(const FVector& Scale)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetRealativeScale3D(Scale);
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
#pragma once
#include "Core/Object/Object.h"
#include "WorldPartition/DebugHelpers.h"
#include "EngineDefine.h"
#include "Classes/Components/SceneComponent.h"

class UActorComponent;

/**
 *	���� ���� Ŭ���� 
 */
class AActor : public UObject
{
	friend class ULevel;
public:
	/** ������, �Ҹ��� */
	ENGINE_API AActor();
	ENGINE_API ~AActor();

	/** ��ü �� ���� ���� */
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
		/** ���� ������Ʈ�� ��ӹ޾Ҵ��� �˻� */
		static_assert(std::is_base_of_v<UActorComponent, ComponentType>, 
			"���� ������Ʈ�� ��ӹ��� ���� Ŭ������ CreateDefaultSubObject�Ϸ��� �õ�");
		if (false == std::is_base_of_v<UActorComponent, ComponentType>)
		{
			MSGASSERT("���� ������Ʈ�� ��ӹ��� ���� Ŭ������ CreateDefaultSubObject�Ϸ��� �õ�");
			return nullptr;
		}

		char* ComponentMemory = new char[sizeof(ComponentType)];

		UActorComponent* ComponentPtr = reinterpret_cast<ComponentType*>(ComponentMemory);
		ComponentPtr->SetOwner(this);

		ComponentType* NewPtr = reinterpret_cast<ComponentType*>(ComponentMemory);

		std::shared_ptr<ComponentType> NewComponent(new(ComponentMemory)ComponentType());

		/** ������Ʈ�� SceneComponent�� ��� */
		if (std::is_base_of_v<USceneComponent, ComponentType>)
		{
			if (nullptr != RootComponent)
			{
				MSGASSERT("Root component�� �Ҵ���� �ʾҽ��ϴ�.");
			}

			RootComponent = NewComponent;
		}
		else if (std::is_base_of_v<UActorComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewComponent);
		}
		else
		{
			MSGASSERT("� ������Ʈ�� �ƴմϴ�.");
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
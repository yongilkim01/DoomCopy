#pragma once
#include "Core/Object/Object.h"
#include "WorldPartition/DebugHelpers.h"
#include "EngineDefine.h"

class USceneComponent;
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
	void CreateDefaultSubObject()
	{
		static_assert(std::is_base_of_v<UActorComponent, ComponentType>, "���� ������Ʈ�� ��ӹ��� ���� Ŭ������ CreateDefaultSubObject�Ϸ��� �õ�");

		if (false == std::is_base_of_v<UActorComponent, ComponentType>)
		{
			MSGASSERT("ActorComponent�� ��ӹ��� ���� Ŭ������ CreateDefaultSubObject �õ�");
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
				MSGASSERT("���� ���ϱ����� ������ �ʾҽ��ϴ�.");
			}

			RootComponent = NewComponent;
		}
		else if (std::is_base_of_v<UActorComponent, ComponentType>)
		{

		}
		else
		{
			MSGASSERT("���� �ȵ�");
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
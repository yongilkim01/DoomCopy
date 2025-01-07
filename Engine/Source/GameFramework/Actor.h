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

	ENGINE_API void AttachToActor(AActor* Parent);

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
		if (std::is_base_of_v<UActorComponent, ComponentType>
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewComponent);
		}
		else if(!std::is_base_of_v<UActorComponent, ComponentType>
				&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			MSGASSERT("� ������Ʈ�� �ƴմϴ�.");
		}

		return NewComponent;
	}





	ENGINE_API FVector GetActorUpVector();
	ENGINE_API FVector GetActorRightVector();
	ENGINE_API FVector GetActorForwardVector();

	ENGINE_API void SetActorLocation(const FVector& NewLocation)
	{
		if (nullptr == RootComponent)
		{
			return;
		}
		RootComponent->SetWorldLocation(NewLocation);
	}
	ENGINE_API void AddActorLocation(const FVector& DeltaLocation)
	{
		if (nullptr == RootComponent)
		{
			return;
		}
		RootComponent->AddWorldLocation(DeltaLocation);
	}
	ENGINE_API void SetActorRotation(const FVector& NewRotation)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetWorldRotation(NewRotation);
	}
	ENGINE_API void AddActorRotation(const FVector& DeltaRotation)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddWorldRotation(DeltaRotation);
	}

	ENGINE_API void SetActorRelativeScale3D(const FVector& Scale)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetRelativeScale3D(Scale); 
	}
	ENGINE_API void AddActorRelativeLocation(const FVector& DeltaLocation)
	{
		if (nullptr == RootComponent)
		{
			return;
		}
		RootComponent->AddRelativeLocation(DeltaLocation);
	}

	ENGINE_API ULevel* GetWorld()
	{
		return World;
	}

	ENGINE_API std::shared_ptr<USceneComponent> GetRootComponent() const
	{
		return RootComponent;
	}
	ENGINE_API void SetRootComponent(const std::shared_ptr<USceneComponent> NewRootComponent)
	{
		RootComponent = NewRootComponent;
	}
	ENGINE_API FTransform GetActorTransform()
	{
		if (nullptr == RootComponent)
		{
			return FTransform();
		}
		return RootComponent->GetComponentTransform();
	}
	
	ENGINE_API FVector GetActorLocation()
	{
		if (nullptr == RootComponent)
		{
			return FVector();
		}
		return RootComponent->Transform.WorldLocation;
	}

protected:
	std::shared_ptr<USceneComponent> RootComponent = nullptr;

private:
	ULevel* World;

	std::list<std::shared_ptr<UActorComponent>> ActorComponentList;

};
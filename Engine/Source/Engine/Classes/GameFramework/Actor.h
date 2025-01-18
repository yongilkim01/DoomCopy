#pragma once
#include "Engine/Classes/Engine/Object.h"
#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/GameFramework/MovementComponent.h"
#include "Engine/Classes/Engine/GameEngine.h"

#include "Core/Public/Debugging/DebugMacros.h"

#include "Physics/Public/PhysicsCore.h"

#include "EngineDefine.h"

class UActorComponent;
class UGameInstance;

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

	ENGINE_API void AttachToActor(AActor* Parent);

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

		size_t ComponentTypeSize = sizeof(ComponentType);

		char* ComponentMemory = new char[sizeof(ComponentType)];

		UActorComponent* ComponentPtr = reinterpret_cast<ComponentType*>(ComponentMemory);
		ComponentPtr->SetOwner(this);

		ComponentType* NewPtr = reinterpret_cast<ComponentType*>(ComponentMemory);

		std::shared_ptr<ComponentType> NewComponent(new(ComponentMemory)ComponentType());

		AllComponentList.push_back(NewComponent);

		/** 컴포넌트가 SceneComponent일 경우 */
		if (std::is_base_of_v<UActorComponent, ComponentType>
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewComponent);
		}
		else if (std::is_base_of_v<UMovementComponent, ComponentType>
				 && !std::is_base_of_v<UActorComponent, ComponentType>
				 && !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewComponent);
		}
		else if(!std::is_base_of_v<UActorComponent, ComponentType>
				&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			MSGASSERT("어떤 컴포넌트도 아닙니다.");
		}

		return NewComponent;
	}

	template<typename ComponentType>
	std::vector<std::shared_ptr<ComponentType>> GetComponentByClass()
	{
		std::vector<std::shared_ptr<ComponentType>> Result;

		for (std::shared_ptr<class UActorComponent> Component : AllComponentList)
		{
			std::shared_ptr<ComponentType> Com = std::dynamic_pointer_cast<ComponentType>(Component);
			if (nullptr != Com)
			{
				Result.push_back(Com);
			}
		}
		return Result;
	}

	template<typename GameInstanceType>
	GameInstanceType* GetGameInstance()
	{
		return dynamic_cast<GameInstanceType*>(GetGameInstance());
	}
	ENGINE_API UGameInstance* GetGameInstance();

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
	ENGINE_API void SetActorTransform(const FTransform& NewTransform)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->Transform = NewTransform;

		return;
	}

	ENGINE_API FVector GetActorLocation()
	{
		if (nullptr == RootComponent)
		{
			return FVector();
		}
		return RootComponent->Transform.WorldLocation;
	}
	ENGINE_API bool IsJumping()
	{
		return bIsJumping;
	}
	ENGINE_API void SetIsJumping(bool Value)
	{
		bIsJumping = Value;
	}
	ENGINE_API FVector GetPerformMovement()
	{
		return PerformMovement;
	}
	ENGINE_API void SetPerformMovement(FVector Value)
	{
		PerformMovement = Value;
	}
	ENGINE_API FVector GetPerformRotation()
	{
		return PerformRotation;
	}
	ENGINE_API void SetPerformRotation(FVector Value)
	{
		PerformRotation = Value;
	}

protected:
	std::shared_ptr<USceneComponent> RootComponent = nullptr;

	bool bIsJumping = false;

	FVector PerformMovement = FVector::ZERO;
	FVector PerformRotation = FVector::ZERO;

	std::list<std::shared_ptr<UActorComponent>> ActorComponentList;

private:
	ULevel* World;

	/** 레퍼런스 카운트 유지용 자료구조 */ 
	std::list<std::shared_ptr<UActorComponent> > AllComponentList;

	AActor* Parent = nullptr;
	std::list<std::shared_ptr<AActor>> ChildList;

};
#pragma once
#include "Engine/Classes/Engine/Object.h"

class AActor;
class ULevel;

/**
 *	설명
 */
class UActorComponent : public UObject
{
	friend AActor;

public:
	/** 생성자, 소멸자 */
	UActorComponent();
	~UActorComponent();

	/** 객체 값 복사 방지 */
	UActorComponent(const UActorComponent& Other) = delete;
	UActorComponent(UActorComponent&& Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& Other) = delete;
	UActorComponent& operator=(UActorComponent&& Other) noexcept = delete;

	ENGINE_API virtual void InitializeComponent() {}
	ENGINE_API virtual void BeginPlay() {}
	ENGINE_API virtual void TickComponent(float DeltaTime) {}

	template<typename ConvertType>
	ConvertType* GetOwner()
	{
		return dynamic_cast<ConvertType*>(GetOwner());
	}
	ENGINE_API ULevel* GetWorld();
	ENGINE_API AActor* GetOwner() { return Owner; }
	ENGINE_API void SetOwner(AActor* NewOwner) { Owner = NewOwner; }
	ENGINE_API virtual bool IsActive() override;
	ENGINE_API virtual bool IsDestroy() override;

protected:

private:
	AActor* Owner;

};
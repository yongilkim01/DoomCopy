#pragma once

#include "Engine/Classes/Components/ActorComponent.h"

struct FTimeEvent
{
public:
	bool Loop = false;
	float CurTime = 0.0f;
	float MaxTime = 0.0f;
	std::function<void(float, float)> TimeUpdateEvent;
	std::function<void()> TimeEndEvent;
};

/**
 *	
 */
class UTimerManager : public UActorComponent
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UTimerManager();
	ENGINE_API ~UTimerManager();

	/** ��ü �� ���� ���� */
	UTimerManager(const UTimerManager& _Other) = delete;
	UTimerManager(UTimerManager&& _Other) noexcept = delete;
	UTimerManager& operator=(const UTimerManager& _Other) = delete;
	UTimerManager& operator=(UTimerManager&& _Other) noexcept = delete;

	/** TimerManager �̺�Ʈ �޼ҵ� */
	ENGINE_API void AddEvent(float Time, std::function<void(float, float)> TimerUpdateEvent, std::function<void()> TimeEndEvent, bool bLoop = false);
	ENGINE_API void AddEndEvent(float Time, std::function<void()> TimeEndEvent, bool bLoop = false);
	ENGINE_API void AddUpdateEvent(float Time, std::function<void(float, float)> TimeUpdateEvent, bool bLoop = false);

protected:
	ENGINE_API void BeginPlay() override;
	ENGINE_API void ComponentTick(float _DeltaTime) override;

private:
	std::list<FTimeEvent> Events;
};
#include "pch.h"
#include "Engine/Public/TimerManager.h"

UTimerManager::UTimerManager()
{
}

UTimerManager::~UTimerManager()
{
}

void UTimerManager::BeginPlay()
{
}

void UTimerManager::TickComponent(float DeltaTime)
{
	UActorComponent::TickComponent(DeltaTime);

	for (FTimeEvent& TimeEvent : Events)
	{
		TimeEvent.CurTime += DeltaTime;

		if (nullptr != TimeEvent.TimeUpdateEvent)
		{
			TimeEvent.TimeUpdateEvent(DeltaTime, TimeEvent.CurTime);
		}

		if (TimeEvent.CurTime > TimeEvent.MaxTime)
		{
			if (nullptr == TimeEvent.TimeEndEvent)
			{
				continue;
			}
			TimeEvent.TimeEndEvent();
		}
	}

	std::list<FTimeEvent>::iterator StartIter = Events.begin();
	std::list<FTimeEvent>::iterator EndIter = Events.end();

	for (; StartIter != EndIter; )
	{
		FTimeEvent& TimeEvent = *StartIter;

		if (TimeEvent.CurTime < TimeEvent.MaxTime)
		{
			++StartIter;
			continue;
		}

		if (false == TimeEvent.Loop)
		{
			StartIter = Events.erase(StartIter);
		}
		else
		{
			++StartIter;
			TimeEvent.CurTime = 0.0f;
		}
	}
}


void UTimerManager::AddEvent(float Time, std::function<void(float, float)> TimerUpdateEvent, std::function<void()> TimeEndEvent, bool bLoop)
{
	FTimeEvent& NewEvent = Events.emplace_back();

	NewEvent.MaxTime = Time;
	NewEvent.CurTime = 0.0f;
	NewEvent.Loop = bLoop;
	NewEvent.TimeEndEvent = TimeEndEvent;
	NewEvent.TimeUpdateEvent = TimerUpdateEvent;
}

void UTimerManager::AddEndEvent(float Time, std::function<void()> TimeEndEvent, bool bLoop)
{
	AddEvent(Time, nullptr, TimeEndEvent, bLoop);
}

void UTimerManager::AddUpdateEvent(float Time, std::function<void(float, float)> TimeUpdateEvent, bool bLoop)
{
	AddEvent(Time, TimeUpdateEvent, nullptr, bLoop);
}
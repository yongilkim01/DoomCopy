#pragma once
#include <functional>

class TimeEventFunction
{
public:
	float Time = 0.0f;
	float MaxTime = 0.0f;
	std::function<void()> Event;
	bool IsUpdate = false;
	bool Loop = false;
};

/**
 *	설명
 */
class UTimeEvent
{
public:
	/** 생성자, 소멸자 */
	UTimeEvent();
	~UTimeEvent();

	/** 객체 값 복사 방지 */
	UTimeEvent(const UTimeEvent& Other) = delete;
	UTimeEvent(UTimeEvent&& Other) noexcept = delete;
	UTimeEvent& operator=(const UTimeEvent& Other) = delete;
	UTimeEvent& operator=(UTimeEvent&& Other) noexcept = delete;

	void PushEvent(float Time, std::function<void()> Function, bool IsUpdate = false, bool IsLoop = false)
	{
		Events.push_front({ Time, Time, Function, IsUpdate, IsLoop });
	}

	void Update(float DeltaTime)
	{
		std::list<TimeEventFunction>::iterator StartIter = Events.begin();
		std::list<TimeEventFunction>::iterator EndIter = Events.end();

		for (; StartIter != EndIter;)
		{
			TimeEventFunction& TimeEvent = *StartIter;
			TimeEvent.Time -= DeltaTime;

			if (true == TimeEvent.IsUpdate && 0.0f < TimeEvent.Time)
			{
				TimeEvent.Event();
			}

			if (0.0f >= TimeEvent.Time)
			{
				TimeEvent.Event();
				if (false == TimeEvent.Loop)
				{
					StartIter = Events.erase(StartIter);
				}
				else
				{
					++StartIter;
					TimeEvent.Time = TimeEvent.MaxTime;
				}
			}
			else
			{
				StartIter++;
			}
		}
	}

protected:

private:
	std::list<TimeEventFunction> Events;
};


#pragma once
#include <WorldPartition/DebugHelpers.h>

#include <vector>
#include <functional>
#include <map>

enum class KeyEvent
{
	Down,
	Press,
	Free,
	Up,
};

/**
 *	설명
 */
class UEngineInput
{
public:
	/** 생성자, 소멸자 */
	~UEngineInput();

	/** 객체 값 복사 방지 */
	UEngineInput(const UEngineInput& Other) = delete;
	UEngineInput(UEngineInput&& Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput& Other) = delete;
	UEngineInput& operator=(UEngineInput&& Other) noexcept = delete;

private:
	class UEngineKey
	{
	public:
		int Key = -1;
		// 키가 안눌리다가 처음 눌렸을때
		bool IsDown = false;
		// 키가 눌린 이후로 계속 누르고 있을때
		bool IsPress = false;
		// 키가 눌리다가 땠을때
		bool IsUp = false;
		// 키가 안누르고 있을때
		bool IsFree = true;
		float PressTime = 0.0f;
		float FreeTime = 0.0f;
		std::vector<std::function<void()>> PressEvents;
		std::vector<std::function<void()>> DownEvents;
		std::vector<std::function<void()>> UpEvents;
		std::vector<std::function<void()>> FreeEvents;
		// 벡터나 리스트를 사용하면 에러가 난다.
		// 학생들이 대처를 못하는데
		// 결국 맨위로 올라가면 됩니다.
		UEngineKey()
		{
		}
		UEngineKey(int _Key)
			: Key(_Key)
		{
		}
		void EventCheck();
		void KeyCheck(float _DeltaTime);

	};
	ENGINE_API static UEngineInput& GetInst();
	ENGINE_API static void EventCheck(float _DeltaTime);

public:
	ENGINE_API static void KeyCheck(float _DeltaTime);
	// UEngineInput::GetInst().IsDown('A')
	bool IsDoubleClick(int _KeyIndex, float _Time)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}
		return Keys[_KeyIndex].IsDown && Keys[_KeyIndex].FreeTime < _Time;
	}
	static bool IsDown(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}
		// 
		// Keys[_KeyIndex] => 없으면 내부에서 노드를 만든다.
		// UMapNode
		// {
		//     UEngineKey Value = UEngineKey();
		// }
		return GetInst().Keys[_KeyIndex].IsDown;
	}
	bool IsUp(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}
		return GetInst().Keys[_KeyIndex].IsUp;
	}
	static bool IsPress(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}
		return GetInst().Keys[_KeyIndex].IsPress;
	}
	float IsPressTime(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}
		return GetInst().Keys[_KeyIndex].PressTime;
	}
	bool IsFree(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}
		return GetInst().Keys[_KeyIndex].IsFree;
	}
	void BindAction(int _KeyIndex, KeyEvent _EventType, std::function<void()> _Function);

protected:

private:
	// 이러면 값형
	// static 값형 변수 선언하고 
	// 지울필요 없다.
	// static UEngineInput Inst;

	// 포인터형
	// static UEngineInput* Inst;
	// 싱글톤의 정의
	// 싱글톤 만들기 1
	// 1. 생성자를 private으로 막습니다.
	// 100개까지는 
	std::map<int, UEngineKey> Keys;
	UEngineInput();
};
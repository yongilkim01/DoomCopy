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
 *	����
 */
class UEngineInput
{
public:
	/** ������, �Ҹ��� */
	~UEngineInput();

	/** ��ü �� ���� ���� */
	UEngineInput(const UEngineInput& Other) = delete;
	UEngineInput(UEngineInput&& Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput& Other) = delete;
	UEngineInput& operator=(UEngineInput&& Other) noexcept = delete;

private:
	class UEngineKey
	{
	public:
		int Key = -1;
		// Ű�� �ȴ����ٰ� ó�� ��������
		bool IsDown = false;
		// Ű�� ���� ���ķ� ��� ������ ������
		bool IsPress = false;
		// Ű�� �����ٰ� ������
		bool IsUp = false;
		// Ű�� �ȴ����� ������
		bool IsFree = true;
		float PressTime = 0.0f;
		float FreeTime = 0.0f;
		std::vector<std::function<void()>> PressEvents;
		std::vector<std::function<void()>> DownEvents;
		std::vector<std::function<void()>> UpEvents;
		std::vector<std::function<void()>> FreeEvents;
		// ���ͳ� ����Ʈ�� ����ϸ� ������ ����.
		// �л����� ��ó�� ���ϴµ�
		// �ᱹ ������ �ö󰡸� �˴ϴ�.
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
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}
		return Keys[_KeyIndex].IsDown && Keys[_KeyIndex].FreeTime < _Time;
	}
	static bool IsDown(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}
		// 
		// Keys[_KeyIndex] => ������ ���ο��� ��带 �����.
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
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}
		return GetInst().Keys[_KeyIndex].IsUp;
	}
	static bool IsPress(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}
		return GetInst().Keys[_KeyIndex].IsPress;
	}
	float IsPressTime(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}
		return GetInst().Keys[_KeyIndex].PressTime;
	}
	bool IsFree(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("������ ��ϵ��� ���� Ű�� �����մϴ�.");
			return false;
		}
		return GetInst().Keys[_KeyIndex].IsFree;
	}
	void BindAction(int _KeyIndex, KeyEvent _EventType, std::function<void()> _Function);

protected:

private:
	// �̷��� ����
	// static ���� ���� �����ϰ� 
	// �����ʿ� ����.
	// static UEngineInput Inst;

	// ��������
	// static UEngineInput* Inst;
	// �̱����� ����
	// �̱��� ����� 1
	// 1. �����ڸ� private���� �����ϴ�.
	// 100�������� 
	std::map<int, UEngineKey> Keys;
	UEngineInput();
};
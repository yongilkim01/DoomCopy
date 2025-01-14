#pragma once
#include <functional>

/**
 * Delegate Ŭ����
 */
class UDelegate
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UDelegate();
	ENGINE_API UDelegate(std::function<void()> Function)
	{
		Functions.push_back(Function);
	}
	ENGINE_API ~UDelegate();

	void operator+=(std::function<void()> Function)
	{
		Functions.push_back(Function);
	}
	void operator()()
	{
		std::list<std::function<void()>>::iterator StartIter = Functions.begin();
		std::list<std::function<void()>>::iterator EndIter = Functions.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			std::function<void()>& Function = *StartIter;
			Function();
		}
	}

	/** Delegate �޼ҵ� */
	void Clear()
	{
		Functions.clear();
	}
	bool IsBind()
	{
		return false == Functions.empty();
	}

protected:

private:
	std::list<std::function<void()>> Functions;
};


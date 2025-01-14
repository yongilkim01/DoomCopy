#pragma once
#include <Core/Public/Math/EngineMath.h>
#include <Core/Public/Debugging/DebugMacros.h>

class ULevel;

struct UEngineInitData
{
public:
	FVector WindowPosition;
	FVector WindowSize;
};

class IContentsCore
{
public:
	virtual ~IContentsCore() = 0 {};

	virtual void EngineStart(UEngineInitData& Data) = 0 {};
	virtual void EngineTick(float DeltaTime) {};
	virtual void EngineEnd() {};
};

#define CreateContentsCoreDefine(TYPE) \
STDAPI_(__declspec(dllexport) INT_PTR) __stdcall CreateContentsCore(std::shared_ptr<IContentsCore>& Test) \
{ \
	Test = std::make_shared<TYPE>(); \
	if (nullptr == Test) \
	{ \
		MSGASSERT("������ ��� ������ �����߽��ϴ�."); \
	} \
	return 0; \
}
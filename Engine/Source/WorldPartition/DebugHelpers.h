#pragma once
#include <Windows.h>
#include <assert.h>
#include "EngineDefine.h"

// MessageBoxA: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxa
// assert: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/assert-macro-assert-wassert?view=msvc-170
#define MSGASSERT(Value) \
	std::string ErrorText = std::string(Value); \
	MessageBoxA(nullptr, ErrorText.c_str(), "치명적 에러", MB_OK); assert(false);

namespace UEngineDebug
{
	ENGINE_API void LeakCheck();

	void OutPutString(const std::string& Text);
}


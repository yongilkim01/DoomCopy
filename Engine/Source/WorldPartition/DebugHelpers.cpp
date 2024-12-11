#include "pch.h"
#include "DebugHelpers.h"

namespace UEngineDebug
{
	void OutPutString(const std::string& Text)
	{
		std::string ResultText = Text + "\n";
		OutputDebugStringA(ResultText.c_str());
	}
}
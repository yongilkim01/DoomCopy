#include "Engine.h"
#include "Core/Public/Debugging/DebugMacros.h"


namespace UEngineDebug
{
	// _CrtSetDbgFlag: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/crtsetdbgflag?view=msvc-170
	ENGINE_API void LeakCheck()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	void OutPutString(const std::string& Text)
	{
		std::string ResultText = Text + "\n";
		OutputDebugStringA(ResultText.c_str());
	}

	ENGINE_API void StartConsole()
	{
		AllocConsole();
		FILE* pCout = nullptr;
		FILE* pErr = nullptr;
		FILE* pCin = nullptr;
		errno_t Err;

		Err = freopen_s(&pCout, "CONOUT$", "w", stdout);
		if (pCout != stdout)
		{
			fclose(pCout);
		}
		Err = freopen_s(&pCin, "CONIN$", "r", stdin);
		if (pCin != stdin)
		{
			fclose(pCin);
		}
		Err = freopen_s(&pErr, "CONERR$", "w", stderr);
		if (pErr != stderr)
		{
			fclose(pErr);
		}
	}

	ENGINE_API void EndConsole()
	{
		FreeConsole();
	}
}
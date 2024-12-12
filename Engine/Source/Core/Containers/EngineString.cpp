#include "pch.h"
#include "EngineString.h"
#include "WorldPartition/DebugHelpers.h"

UEngineString::UEngineString()
{
}

UEngineString::~UEngineString()
{
}


std::string UEngineString::ToUpper(std::string_view Str)
{
	std::string Result = Str.data();
	Result.resize(Str.size());

	for (size_t i = 0; i < Str.size(); i++)
	{
		Result[i] = std::toupper(Str[i]);
	}

	return Result;
}

std::wstring UEngineString::AnsiToUnicode(std::string_view Str)
{
	int Size = MultiByteToWideChar(CP_ACP, 0, Str.data(), static_cast<int>(Str.size()), nullptr, 0);

	if (0 == Size)
	{
		MSGASSERT("MultiByteToWideChar 문자열 변환에 실패했습니다" + std::string(Str));
		return L"";
	}

	std::wstring Result;
	Result.resize(Size);

	Size = MultiByteToWideChar(CP_ACP, 0, Str.data(), static_cast<int>(Str.size()), &Result[0], Size);

	if (0 == Size)
	{
		MSGASSERT("MultiByteToWideChar 문자열 변환에 실패했습니다" + std::string(Str));
		return L"";
	}

	return Result;
}



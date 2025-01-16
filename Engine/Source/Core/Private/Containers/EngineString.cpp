#include "pch.h"
#include "Core/Public/Containers/EngineString.h"
#include "Core/Public/Debugging/DebugMacros.h"

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

std::string UEngineString::UniCodeToUTF8(std::wstring_view Text)
{
	int Size = WideCharToMultiByte(CP_UTF8, 0, Text.data(), static_cast<int>(Text.size()), nullptr, 0, nullptr, nullptr);
	if (0 == Size)
	{
		MSGASSERT("문자열 변환에 실패했습니다.");
		return "";
	}

	std::string Result;
	Result.resize(Size);
	Size = WideCharToMultiByte(CP_UTF8, 0, Text.data(), static_cast<int>(Text.size()), &Result[0], Size, nullptr, nullptr);

	if (0 == Size)
	{
		MSGASSERT("문자열 변환에 실패했습니다.");
		return "";
	}
	return Result;
}

std::string UEngineString::AnsiToUTF8(std::string_view Name)
{
	std::wstring WStr = AnsiToUnicode(Name);
	return UniCodeToUTF8(WStr.c_str());
}

std::string UEngineString::InterString(const std::string& Str, std::string_view StartStr, std::string_view EndStr, size_t& Offset)
{
	size_t DataStart = Str.find(StartStr, Offset);
	size_t DataEnd = Str.find(EndStr, DataStart);

	if (DataStart == std::string::npos || DataEnd == std::string::npos)
	{
		return "";
	}


	std::string Result = Str.substr(DataStart + StartStr.size(), (DataEnd - (DataStart + StartStr.size()))).data();

	Offset = DataEnd + 1;
	return Result;
}
#pragma once
#include <string>

#include "Engine/Public/Global/EngineDefine.h"

/**
 *	�������� ����ϴ� ���ڿ� ���� ��� Ŭ����
 */
class UEngineString
{
public:
	~UEngineString();

	/** ��ü �� ���� ���� */
	UEngineString(const UEngineString& Other) = delete;
	UEngineString(UEngineString&& Other) noexcept = delete;
	UEngineString& operator=(const UEngineString& Other) = delete;
	UEngineString& operator=(UEngineString&& Other) noexcept = delete;

	/** �빮�� ��ȯ �޼ҵ� */
	ENGINE_API static std::string ToUpper(std::string_view);
	/** �����ڵ� ��ȯ �޼ҵ� */
	ENGINE_API static std::wstring AnsiToUnicode(std::string_view Name);
	ENGINE_API static std::string UniCodeToUTF8(std::wstring_view Name);
	ENGINE_API static std::string AnsiToUTF8(std::string_view _Name);

	ENGINE_API static std::string InterString(const std::string& Str, std::string_view StartStr,
											  std::string_view EndStr, size_t& Offset);

protected:

private:
	UEngineString();
};


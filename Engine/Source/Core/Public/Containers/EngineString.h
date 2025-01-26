#pragma once
#include <string>

#include "Engine/Public/Global/EngineDefine.h"

/**
 *	엔진에서 사용하는 문자열 관련 기능 클래스
 */
class UEngineString
{
public:
	~UEngineString();

	/** 객체 값 복사 방지 */
	UEngineString(const UEngineString& Other) = delete;
	UEngineString(UEngineString&& Other) noexcept = delete;
	UEngineString& operator=(const UEngineString& Other) = delete;
	UEngineString& operator=(UEngineString&& Other) noexcept = delete;

	/** 대문자 변환 메소드 */
	ENGINE_API static std::string ToUpper(std::string_view);
	/** 유니코드 변환 메소드 */
	ENGINE_API static std::wstring AnsiToUnicode(std::string_view Name);
	ENGINE_API static std::string UniCodeToUTF8(std::wstring_view Name);
	ENGINE_API static std::string AnsiToUTF8(std::string_view _Name);

	ENGINE_API static std::string InterString(const std::string& Str, std::string_view StartStr,
											  std::string_view EndStr, size_t& Offset);

protected:

private:
	UEngineString();
};


#pragma once
#include <string>

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
	static std::string ToUpper(std::string_view);
	/** 유니코드 변환 메소드 */
	static std::wstring AnsiToUnicode(std::string_view Name);
protected:

private:
	UEngineString();
};


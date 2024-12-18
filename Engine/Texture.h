#pragma once

/**
 *	설명
 */
class UTexture
{
public:
	/** 생성자, 소멸자 */
	UTexture();
	~UTexture();

	/** 객체 값 복사 방지 */
	UTexture(const UTexture& _Other) = delete;
	UTexture(UTexture&& _Other) noexcept = delete;
	UTexture& operator=(const UTexture& _Other) = delete;
	UTexture& operator=(UTexture&& _Other) noexcept = delete;

protected:

private:

};


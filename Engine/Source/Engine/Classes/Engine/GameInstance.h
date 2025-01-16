#pragma once

/**
 *	설명
 */
class UGameInstance
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UGameInstance();
	ENGINE_API ~UGameInstance();

	/** 객체 값 복사 방지 */
	UGameInstance(const UGameInstance& Other) = delete;
	UGameInstance(UGameInstance&& Other) noexcept = delete;
	UGameInstance& operator=(const UGameInstance& Other) = delete;
	UGameInstance& operator=(UGameInstance&& Other) noexcept = delete;

protected:

private:

};


#pragma once

/**
 *	설명
 */
class UPaperSpriteComponent
{
public:
	/** 생성자, 소멸자 */
	UPaperSpriteComponent();
	~UPaperSpriteComponent();

	/** 객체 값 복사 방지 */
	UPaperSpriteComponent(const UPaperSpriteComponent& Other) = delete;
	UPaperSpriteComponent(UPaperSpriteComponent&& Other) noexcept = delete;
	UPaperSpriteComponent& operator=(const UPaperSpriteComponent& Other) = delete;
	UPaperSpriteComponent& operator=(UPaperSpriteComponent&& Other) noexcept = delete;

protected:

private:

};


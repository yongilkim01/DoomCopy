#pragma once

/**
 *	����
 */
class UPaperSpriteComponent
{
public:
	/** ������, �Ҹ��� */
	UPaperSpriteComponent();
	~UPaperSpriteComponent();

	/** ��ü �� ���� ���� */
	UPaperSpriteComponent(const UPaperSpriteComponent& Other) = delete;
	UPaperSpriteComponent(UPaperSpriteComponent&& Other) noexcept = delete;
	UPaperSpriteComponent& operator=(const UPaperSpriteComponent& Other) = delete;
	UPaperSpriteComponent& operator=(UPaperSpriteComponent&& Other) noexcept = delete;

protected:

private:

};


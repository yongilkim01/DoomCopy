#pragma once

/**
 *	����
 */
class UTexture
{
public:
	/** ������, �Ҹ��� */
	UTexture();
	~UTexture();

	/** ��ü �� ���� ���� */
	UTexture(const UTexture& _Other) = delete;
	UTexture(UTexture&& _Other) noexcept = delete;
	UTexture& operator=(const UTexture& _Other) = delete;
	UTexture& operator=(UTexture&& _Other) noexcept = delete;

protected:

private:

};


#pragma once

/**
 *	����
 */
class UGameInstance
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UGameInstance();
	ENGINE_API ~UGameInstance();

	/** ��ü �� ���� ���� */
	UGameInstance(const UGameInstance& Other) = delete;
	UGameInstance(UGameInstance&& Other) noexcept = delete;
	UGameInstance& operator=(const UGameInstance& Other) = delete;
	UGameInstance& operator=(UGameInstance&& Other) noexcept = delete;

protected:

private:

};


#pragma once

/**
 *	����
 */
class UEngineMaterial
{
public:
	/** ������, �Ҹ��� */
	UEngineMaterial();
	~UEngineMaterial();

	/** ��ü �� ���� ���� */
	UEngineMaterial(const UEngineMaterial& Other) = delete;
	UEngineMaterial(UEngineMaterial&& Other) noexcept = delete;
	UEngineMaterial& operator=(const UEngineMaterial& Other) = delete;
	UEngineMaterial& operator=(UEngineMaterial&& Other) noexcept = delete;

protected:

private:

};


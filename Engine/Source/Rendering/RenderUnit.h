#pragma once

/**
 *	����
 */
class URenderUnit
{
public:
	/** ������, �Ҹ��� */
	URenderUnit();
	~URenderUnit();

	/** ��ü �� ���� ���� */
	URenderUnit(const URenderUnit& Other) = delete;
	URenderUnit(URenderUnit&& Other) noexcept = delete;
	URenderUnit& operator=(const URenderUnit& Other) = delete;
	URenderUnit& operator=(URenderUnit&& Other) noexcept = delete;

protected:

private:

};


#pragma once

/**
 *	����
 */
class URenderUnit
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API URenderUnit();
	ENGINE_API ~URenderUnit();

	/** ��ü �� ���� ���� */
	URenderUnit(const URenderUnit& Other) = delete;
	URenderUnit(URenderUnit&& Other) noexcept = delete;
	URenderUnit& operator=(const URenderUnit& Other) = delete;
	URenderUnit& operator=(URenderUnit&& Other) noexcept = delete;

protected:

private:

};


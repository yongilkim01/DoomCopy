#pragma once
#include <d3d11_4.h>

/**
 *	����
 */
class EngineGraphicDevice
{
public:
	/** ������, �Ҹ��� */
	EngineGraphicDevice();
	~EngineGraphicDevice();

	/** ��ü �� ���� ���� */
	EngineGraphicDevice(const EngineGraphicDevice& Other) = delete;
	EngineGraphicDevice(EngineGraphicDevice&& Other) noexcept = delete;
	EngineGraphicDevice& operator=(const EngineGraphicDevice& Other) = delete;
	EngineGraphicDevice& operator=(EngineGraphicDevice&& Other) noexcept = delete;

protected:

private:

};


#pragma once
#include <d3d11_4.h>

/**
 *	설명
 */
class EngineGraphicDevice
{
public:
	/** 생성자, 소멸자 */
	EngineGraphicDevice();
	~EngineGraphicDevice();

	/** 객체 값 복사 방지 */
	EngineGraphicDevice(const EngineGraphicDevice& Other) = delete;
	EngineGraphicDevice(EngineGraphicDevice&& Other) noexcept = delete;
	EngineGraphicDevice& operator=(const EngineGraphicDevice& Other) = delete;
	EngineGraphicDevice& operator=(EngineGraphicDevice&& Other) noexcept = delete;

protected:

private:

};


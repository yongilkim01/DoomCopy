#pragma once

/**
 *	설명
 */
class UEngineDeviceBuffer
{
public:
	/** 생성자, 소멸자 */
	UEngineDeviceBuffer();
	~UEngineDeviceBuffer();

	/** 객체 값 복사 방지 */
	UEngineDeviceBuffer(const UEngineDeviceBuffer& Other) = delete;
	UEngineDeviceBuffer(UEngineDeviceBuffer&& Other) noexcept = delete;
	UEngineDeviceBuffer& operator=(const UEngineDeviceBuffer& Other) = delete;
	UEngineDeviceBuffer& operator=(UEngineDeviceBuffer&& Other) noexcept = delete;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer = nullptr;
	D3D11_BUFFER_DESC BufferInfo = { 0 };

private:

};


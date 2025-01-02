#pragma once

/**
 *	����
 */
class UEngineDeviceBuffer
{
public:
	/** ������, �Ҹ��� */
	UEngineDeviceBuffer();
	~UEngineDeviceBuffer();

	/** ��ü �� ���� ���� */
	UEngineDeviceBuffer(const UEngineDeviceBuffer& Other) = delete;
	UEngineDeviceBuffer(UEngineDeviceBuffer&& Other) noexcept = delete;
	UEngineDeviceBuffer& operator=(const UEngineDeviceBuffer& Other) = delete;
	UEngineDeviceBuffer& operator=(UEngineDeviceBuffer&& Other) noexcept = delete;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer = nullptr;
	D3D11_BUFFER_DESC BufferInfo = { 0 };

private:

};


#pragma once

/**
 *	����
 */
class UEnginePixelShader
{
public:
	/** ������, �Ҹ��� */
	UEnginePixelShader();
	~UEnginePixelShader();

	/** ��ü �� ���� ���� */
	UEnginePixelShader(const UEnginePixelShader& Other) = delete;
	UEnginePixelShader(UEnginePixelShader&& Other) noexcept = delete;
	UEnginePixelShader& operator=(const UEnginePixelShader& Other) = delete;
	UEnginePixelShader& operator=(UEnginePixelShader&& Other) noexcept = delete;

protected:

private:

};


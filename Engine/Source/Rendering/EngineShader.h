#pragma once

/**
 *	����
 */
class UEngineShader
{
public:
	/** ������, �Ҹ��� */
	UEngineShader();
	~UEngineShader();

	/** ��ü �� ���� ���� */
	UEngineShader(const UEngineShader& Other) = delete;
	UEngineShader(UEngineShader&& Other) noexcept = delete;
	UEngineShader& operator=(const UEngineShader& Other) = delete;
	UEngineShader& operator=(UEngineShader&& Other) noexcept = delete;

protected:

private:

};


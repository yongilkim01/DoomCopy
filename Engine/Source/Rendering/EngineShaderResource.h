#pragma once

/**
 *	����
 */
class UEngineShaderResource
{
public:
	/** ������, �Ҹ��� */
	UEngineShaderResource();
	~UEngineShaderResource();

	/** ��ü �� ���� ���� */
	UEngineShaderResource(const UEngineShaderResource& Other) = delete;
	UEngineShaderResource(UEngineShaderResource&& Other) noexcept = delete;
	UEngineShaderResource& operator=(const UEngineShaderResource& Other) = delete;
	UEngineShaderResource& operator=(UEngineShaderResource&& Other) noexcept = delete;

protected:

private:

};


#pragma once

/**
 *	����
 */
class UEngineRasterizerState
{
public:
	/** ������, �Ҹ��� */
	UEngineRasterizerState();
	~UEngineRasterizerState();

	/** ��ü �� ���� ���� */
	UEngineRasterizerState(const UEngineRasterizerState& Other) = delete;
	UEngineRasterizerState(UEngineRasterizerState&& Other) noexcept = delete;
	UEngineRasterizerState& operator=(const UEngineRasterizerState& Other) = delete;
	UEngineRasterizerState& operator=(UEngineRasterizerState&& Other) noexcept = delete;

protected:

private:

};


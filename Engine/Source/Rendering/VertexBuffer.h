#pragma once

/**
 *	����
 */
class FVertexBuffer
{
public:
	/** ������, �Ҹ��� */
	FVertexBuffer();
	~FVertexBuffer();

	/** ��ü �� ���� ���� */
	FVertexBuffer(const FVertexBuffer& Other) = delete;
	FVertexBuffer(FVertexBuffer&& Other) noexcept = delete;
	FVertexBuffer& operator=(const FVertexBuffer& Other) = delete;
	FVertexBuffer& operator=(FVertexBuffer&& Other) noexcept = delete;

protected:

private:

};


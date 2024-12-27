#pragma once

/**
 *	설명
 */
class FVertexBuffer
{
public:
	/** 생성자, 소멸자 */
	FVertexBuffer();
	~FVertexBuffer();

	/** 객체 값 복사 방지 */
	FVertexBuffer(const FVertexBuffer& Other) = delete;
	FVertexBuffer(FVertexBuffer&& Other) noexcept = delete;
	FVertexBuffer& operator=(const FVertexBuffer& Other) = delete;
	FVertexBuffer& operator=(FVertexBuffer&& Other) noexcept = delete;

protected:

private:

};


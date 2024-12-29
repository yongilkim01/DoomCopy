#pragma once

/**
 *	DoomIndexBuffer File class
 */
class FDoomIndexBuffer
{
public:
	/** 생성자, 소멸자 */
	FDoomIndexBuffer();
	~FDoomIndexBuffer();

	/** 객체 값 복사방지 */
	FDoomIndexBuffer(const FDoomIndexBuffer& Other) = delete;
	FDoomIndexBuffer(FDoomIndexBuffer&& Other) noexcept = delete;
	FDoomIndexBuffer& operator=(const FDoomIndexBuffer& Other) = delete;
	FDoomIndexBuffer& operator=(FDoomIndexBuffer&& Other) noexcept = delete;

	/** 겟, 셋 메소드 */

protected:


private:

};


#pragma once

/**
 *	설명
 */
class UEngineMaterial
{
public:
	/** 생성자, 소멸자 */
	UEngineMaterial();
	~UEngineMaterial();

	/** 객체 값 복사 방지 */
	UEngineMaterial(const UEngineMaterial& Other) = delete;
	UEngineMaterial(UEngineMaterial&& Other) noexcept = delete;
	UEngineMaterial& operator=(const UEngineMaterial& Other) = delete;
	UEngineMaterial& operator=(UEngineMaterial&& Other) noexcept = delete;

protected:

private:

};


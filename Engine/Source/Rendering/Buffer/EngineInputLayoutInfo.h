#pragma once

/**
 *	설명
 */
class UEngineInputLayoutInfo
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UEngineInputLayoutInfo();
	ENGINE_API ~UEngineInputLayoutInfo();

	/** 객체 값 복사 방지 */
	UEngineInputLayoutInfo(const UEngineInputLayoutInfo& Other) = delete;
	UEngineInputLayoutInfo(UEngineInputLayoutInfo&& Other) noexcept = delete;
	UEngineInputLayoutInfo& operator=(const UEngineInputLayoutInfo& Other) = delete;
	UEngineInputLayoutInfo& operator=(UEngineInputLayoutInfo&& Other) noexcept = delete;

	ENGINE_API void AddInputLayout(
		LPCSTR _SemanticName, // 내가 포지션이다 내가 컬러다. "POSITION"
		DXGI_FORMAT _Format, // 내가 n바이트 짜리 정보다 자료형
		D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA, // 버텍스 데이터가 인스턴싱 데이터다 라는걸 표현
		UINT _InstanceDataStepRate = 0, // 나중에 나중에 인스턴싱이라는 구조를 배울때 쓸때가 있을것이다.
		UINT _AlignedByteOffset = -1, // 0번째 바이트부터 
		UINT _InputSlot = 0, // n개의 버텍스 버퍼를 세팅할때 n번째 버텍스 버퍼의 인풋 레이아웃이다 라는걸 표현하는것
		UINT _SemanticIndex = 0
	);

	ENGINE_API UINT FormatSize(DXGI_FORMAT _Value);
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutData;

protected:

private:
	int Offset = 0;

};


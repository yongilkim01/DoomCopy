#pragma once

/**
 *	����
 */
class UEngineInputLayoutInfo
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UEngineInputLayoutInfo();
	ENGINE_API ~UEngineInputLayoutInfo();

	/** ��ü �� ���� ���� */
	UEngineInputLayoutInfo(const UEngineInputLayoutInfo& Other) = delete;
	UEngineInputLayoutInfo(UEngineInputLayoutInfo&& Other) noexcept = delete;
	UEngineInputLayoutInfo& operator=(const UEngineInputLayoutInfo& Other) = delete;
	UEngineInputLayoutInfo& operator=(UEngineInputLayoutInfo&& Other) noexcept = delete;

	ENGINE_API void AddInputLayout(
		LPCSTR _SemanticName, // ���� �������̴� ���� �÷���. "POSITION"
		DXGI_FORMAT _Format, // ���� n����Ʈ ¥�� ������ �ڷ���
		D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA, // ���ؽ� �����Ͱ� �ν��Ͻ� �����ʹ� ��°� ǥ��
		UINT _InstanceDataStepRate = 0, // ���߿� ���߿� �ν��Ͻ��̶�� ������ ��ﶧ ������ �������̴�.
		UINT _AlignedByteOffset = -1, // 0��° ����Ʈ���� 
		UINT _InputSlot = 0, // n���� ���ؽ� ���۸� �����Ҷ� n��° ���ؽ� ������ ��ǲ ���̾ƿ��̴� ��°� ǥ���ϴ°�
		UINT _SemanticIndex = 0
	);

	ENGINE_API UINT FormatSize(DXGI_FORMAT _Value);
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutData;

protected:

private:
	int Offset = 0;

};


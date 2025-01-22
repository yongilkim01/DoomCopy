// ��� ����(cbuffer)�� ����Ͽ� ��ȯ �����͸� GPU�� ����
// ��� ���۴� �޸� �뷮 ���� ����
cbuffer FTransform : register(b0)
{
    // TransformUpdate �Լ��� �Ʒ��� ������ �����Ͽ�
    // WVP(World-View-Projection) ����� ����

    // ��ȯ�� �ʿ��� ����
    float4 Scale; // ��ü�� �����ϸ� �� (x, y, z �� ������)
    float4 Rotation; // ȸ�� ���� (Euler ���� �Ǵ� Quaternion ��ȯ �� ��)
    float4 Qut; // ȸ���� ���Ǵ� Quaternion ��
    float4 Location; // ��ü�� ��ġ (���� ��ǥ)

    // ��ü�� �θ�� ���õ� �����(Local) ��ȯ ����
    float4 RelativeScale; // �θ� ��ü�� ���� ����� ������
    float4 RelativeRotation; // �θ� ��ü�� ���� ����� ȸ�� (Euler ����)
    float4 RelativeQut; // �θ� ��ü�� ���� ����� ȸ�� (Quaternion ��)
    float4 RelativeLocation; // �θ� ��ü�� ���� ����� ��ġ

    // ���� ��ǥ�迡���� ���� ��ȯ ����
    float4 WorldScale; // ���� ���������� ���� ������ ��
    float4 WorldRotation; // ���� ���������� ���� ȸ�� �� (Euler ����)
    float4 WorldQuat; // ���� ���������� ���� ȸ�� �� (Quaternion ��)
    float4 WorldLocation; // ���� ���������� ���� ��ġ ��

    // ��ȯ ��ĵ�
    float4x4 ScaleMat; // ������ ���
    float4x4 RotationMat; // ȸ�� ���
    float4x4 LocationMat; // ��ġ ��ȯ ���
    float4x4 RevolveMat; // ȸ���� ������� �� ���� ���
    float4x4 ParentMat; // �θ� ��ü�� ��ȯ ���

    // ���� ������ ���� ������ ��ȯ ���
    float4x4 LocalWorld; // ���� �������� ���� ���������� ��ȯ ���
    float4x4 World; // ���� ���������� ���� ��ȯ ���

    // ��� �������� ���
    float4x4 View; // ī�޶��� �� ���
    float4x4 Projection; // ī�޶��� �������� ���

    // WVP(World-View-Projection) ���
    float4x4 WVP; // ���������� GPU���� ���Ǵ� ��ȯ ���
};

// GPU���� ����ϱ� ���� ����ȭ�� ���� ����
struct FTransformStruct
{
    // TransformUpdate �Լ��� �Ʒ��� ������ �����Ͽ�
    // WVP(World-View-Projection) ����� ����

    // ��ȯ�� �ʿ��� ����
    float4 Scale; // ��ü�� �����ϸ� �� (x, y, z �� ������)
    float4 Rotation; // ȸ�� ���� (Euler ���� �Ǵ� Quaternion ��ȯ �� ��)
    float4 Qut; // ȸ���� ���Ǵ� Quaternion ��
    float4 Location; // ��ü�� ��ġ (���� ��ǥ)

    // ��ü�� �θ�� ���õ� �����(Local) ��ȯ ����
    float4 RelativeScale; // �θ� ��ü�� ���� ����� ������
    float4 RelativeRotation; // �θ� ��ü�� ���� ����� ȸ�� (Euler ����)
    float4 RelativeQut; // �θ� ��ü�� ���� ����� ȸ�� (Quaternion ��)
    float4 RelativeLocation; // �θ� ��ü�� ���� ����� ��ġ

    // ���� ��ǥ�迡���� ���� ��ȯ ����
    float4 WorldScale; // ���� ���������� ���� ������ ��
    float4 WorldRotation; // ���� ���������� ���� ȸ�� �� (Euler ����)
    float4 WorldQuat; // ���� ���������� ���� ȸ�� �� (Quaternion ��)
    float4 WorldLocation; // ���� ���������� ���� ��ġ ��

    // ��ȯ ��ĵ�
    float4x4 ScaleMat; // ������ ���
    float4x4 RotationMat; // ȸ�� ���
    float4x4 LocationMat; // ��ġ ��ȯ ���
    float4x4 RevolveMat; // ȸ���� ������� �� ���� ���
    float4x4 ParentMat; // �θ� ��ü�� ��ȯ ���

    // ���� ������ ���� ������ ��ȯ ���
    float4x4 LocalWorld; // ���� �������� ���� ���������� ��ȯ ���
    float4x4 World; // ���� ���������� ���� ��ȯ ���

    // ��� �������� ���
    float4x4 View; // ī�޶��� �� ���
    float4x4 Projection; // ī�޶��� �������� ���

    // WVP(World-View-Projection) ���
    float4x4 WVP; // ���������� GPU���� ���Ǵ� ��ȯ ���
};

// ��ħ ������ �����ϱ� ���� 12�� �������� ����
StructuredBuffer<FTransformStruct> TransformBuffer : register(t0);

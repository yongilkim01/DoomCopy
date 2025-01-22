// �� ��꿡 ���Ǵ� ��� ���� ����ü
// �� ���� ���� ������ �����ϴ� ����ü
struct FLightData
{
    float4 LightPos; // ���� ��ġ (���� ��ǥ)
    float4 LightDir; // ���� ���� ���� ���� (���� ����)
    float4 LightRevDir; // ���� �ݴ� ���� ���� (���� �ݴ� ����)
    float4 LightColor; // ���� ���� (RGB ��)
    float4 AmbientLight; // �ֺ���(Ambient Light)�� ���� (RGB ��)
    float4 ViewLightPos; // ī�޶� �� ���������� ���� ��ġ
    float4 ViewLightDir; // ī�޶� �� ���������� ���� ���� ���� ����
    float4 ViewLightRevDir; // ī�޶� �� ���������� ���� �ݴ� ���� ����
    float4 CameraPosition; // ī�޶��� ��ġ (���� ��ǥ)
};

// �� �����͸� �����ϴ� ��� ���� ����
// Count�� ���� �� ������ ��Ÿ����, LightArr�� �ִ� 256���� �� �����͸� ����
cbuffer FLightDatas : register(b11)
{
    int Count; // ���� ó���� ���� ����
    FLightData LightArr[256]; // �ִ� 256���� �� �����͸� ������ �迭
};

// HLSL������ ���� �Լ��� ����
// HLSL������ ���۷����� ������ ������ ������, ��� ���� �� ���� ������� ó��

// N (�Ϲ������� ���� ����)�� L (���� �ݴ� ���� ����) ���� ������ ���
// Ȯ�� ����(Diffuse Lighting)�� ����ϴ� �Լ�
// �� �Լ��� ���� �ݻ� ������ ����ϴ� �� ���
float4 CalDiffusLight(float4 _Normal, FLightData _Data)
{
    // ���� ���Ϳ� ���� �ݴ� ���� ������ ������ ���� ��,
    // ��� ���� �ش��ϴ� �� ���� ��(RGB)�� ��ȯ
    float4 ResultLight = dot(_Normal, _Data.LightRevDir);

    // �� ���� ��ȯ (���⼭�� �ܼ��� LightColor�� ��ȯ)
    return _Data.LightColor;
}

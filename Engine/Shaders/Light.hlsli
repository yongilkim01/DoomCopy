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


// ����ŧ�� ����(Specular Lighting)�� ����ϴ� �Լ�
// ����ŧ�� ������ ���� �ݻ簡 �����ڿ��� ���� ���� �� �߻��ϴ� ���� ���̶���Ʈ�� �ǹ�
float4 CalSpacularLight(float4 ViewPosition, float4 ViewNormal, FLightData LightData)
{
    // ����� ��ȯ�� ����ŧ�� ������ ����
    float ResultLight = 0.0f;
    
    // ���� ���͸� ����ȭ (Normalize the normal vector)
    float4 N = normalize(ViewNormal);
    
    // ���� �ݴ� ���� ���͸� ����ȭ (Normalize the light reverse direction vector)
    float4 L = normalize(LightData.ViewLightRevDir);
    
    // �ݻ� ���͸� ��� (Reflection vector calculation)
    // �ݻ� ���� = 2 * (N �� L) * N - L
    float3 Reflection = normalize(2.0f * N.xyz * dot(L.xyz, N.xyz) - L.xyz);
    
    // ī�޶�(��) ���� ���͸� ����ȭ (Normalize the eye direction vector)
    float3 Eye = normalize(ViewPosition.xyz - LightData.CameraPosition.xyz);
    
    // �ݻ� ���Ϳ� ī�޶�(��) ���� ������ ������ ��� (Dot product of reflection and eye direction)
    // �� ���� ����ŧ�� ������ ������ ����
    ResultLight = max(0.0f, dot(Reflection.xyz, Eye.xyz));
    
    // ����ŧ�� ������ �� �����ϰ� ����� ���� ���� ���� ���� (Exponent to control the shininess)
    ResultLight = pow(ResultLight, 30.0f);

    // ���� ����ŧ�� ���� ���� ��ȯ
    return ResultLight;
}


// Ȯ�� ����(Diffuse Lighting)�� ����ϴ� �Լ�
// Ȯ�� ������ ��ü�� ǥ���� ���� ���� ���� ������� ȿ���� �ǹ�
float4 CalDiffusLight(float4 ViewNormal, FLightData LightData)
{
    // ���� ���͸� ����ȭ (Normalize the normal vector)
    float4 N = normalize(ViewNormal);
    
    // ���� �ݴ� ���� ���͸� ����ȭ (Normalize the light reverse direction vector)
    float4 L = normalize(LightData.ViewLightRevDir);
    // �� ���ʹ� �� ����(View Space)���� ���ǵǾ�� ������ �ùٸ��� ����

    // Ȯ�� ������ �ʱ� ���� ��
    float ResultLight;

    // ���� ���Ϳ� �� ���� ������ ������ ��� (Dot product of normal and light vectors)
    ResultLight = dot(N, L);

    // ���� ����� ������ ��� �ּҰ��� 0���� ���� (Clamp the result to a minimum of 0)
    ResultLight = max(0.0f, ResultLight);

    // Ȯ�� ���� ������ �����ϱ� ���� �����ϸ� (Scale the diffuse lighting intensity)
    return ResultLight * 0.5f;
}
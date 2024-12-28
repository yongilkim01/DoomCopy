struct VS_INPUT         
{
    float4 POSITION : POSITION;
    float4 COLOR : COLOR;
};

struct VS_OUTPUT
{
    float4 POSITION : SV_POSITION;
    float4 COLOR : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_INPUT output;
    
    output.POSITION = input.POSITION;
    output.COLOR = input.COLOR;
    
    return output;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = input.COLOR;
    
    return color;
}
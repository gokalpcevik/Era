cbuffer CBuf
{
    matrix g_WorldViewProjection;
    float4 g_Color;
};

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV : UV;
};

struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 UV : UV;
    float4 Color : COLOR;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
    PSIn.Position = mul(float4(VSIn.Position,1.0f),g_WorldViewProjection);
    PSIn.Normal = VSIn.Normal;
    PSIn.UV = VSIn.UV;
    PSIn.Color = g_Color;
}


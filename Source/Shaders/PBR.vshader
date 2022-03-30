cbuffer CBuf
{
    matrix g_World;
    matrix g_WorldViewProjection;
    float4 g_Color;
};

struct VSInput
{
    float3 Position : POSITION0;
    float3 Normal : NORMAL0;
    float2 UV : UV0;
};

struct PSInput
{
    float4 Position : SV_Position;
    float4 WorldPosition : WORLD_POSITION;
    float3 Normal : NORMAL;
    float2 UV : UV;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
    PSIn.Position = mul(float4(VSIn.Position, 1.0f), g_WorldViewProjection);
    PSIn.WorldPosition = mul(float4(VSIn.Position, 1.0f), g_World);
    PSIn.Normal = normalize(mul(float4(VSIn.Normal, 0.0f), g_World));
    PSIn.UV = VSIn.UV;
}

cbuffer CBuf
{
    matrix g_World;
    matrix g_WorldViewProjection;
    float4 g_Color;
};

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV : UV;
    bool bUseSamplers : USESAMPLERS;
};

struct PSInput
{
    float4 Position : SV_Position;
    float4 WorldPosition : WORLD_POSITION;
    float3 Normal : NORMAL;
    float2 UV : UV;
    float4 Color : COLOR;
    bool bUseSamplers : USESAMPLERS;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
    PSIn.Position = mul(float4(VSIn.Position,1.0f),g_WorldViewProjection);
    PSIn.WorldPosition = mul(float4(VSIn.Position,1.0f), g_World);
    PSIn.Normal = VSIn.Normal;
    PSIn.UV = VSIn.UV;
    PSIn.Color = g_Color;
    PSIn.bUseSamplers = VSIn.bUseSamplers;
}


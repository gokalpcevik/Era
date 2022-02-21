cbuffer
{
    matrix g_WorldViewProjection;
    float4 g_Color;
};

struct VSInput
{
    float3 Position : Position;
};

struct PSInput
{
    float4 Position : SV_Position;
    float4 Color : Color;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
    PSIn.Position = mul(float4(VSIn.Position,1.0f),g_WorldViewProjection);
    PSIn.Color = g_Color;
}


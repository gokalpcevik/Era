cbuffer UICBuf
{
	float4x4 g_WorldProjection;
    float2 g_Translation;
};

struct VSInput
{
    float4 Position : POSITION;
    float2 UV : UV0;
    float4 Color : COLOR;
};

struct PSInput
{
    float4 Position : SV_Position;
    float2 UV : UV;
    float4 Color : COLOR;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
    PSIn.Position = mul(VSIn.Position , g_WorldProjection);
    PSIn.UV = VSIn.UV;
    PSIn.Color = VSIn.Color;
}
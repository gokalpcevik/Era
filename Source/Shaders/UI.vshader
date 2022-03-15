cbuffer UICBuf
{
	float4x4 g_Transform;
};

struct VSInput
{
    float3 Position : POSITION;
    float2 UV : UV;
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
    PSIn.Position = mul(float4(VSIn.Position, 1.0f), g_Transform);
    PSIn.UV = VSIn.UV;
    PSIn.Color = VSIn.Color;
}
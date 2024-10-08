
struct VSOut
{
    float3 color : Color;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    //row_major matrix transform;
    matrix transform;
};

VSOut main(float3 pos : POSITION, float3 color : Color)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform);
    vso.color = color;
    return vso;
}

struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};


Texture2D u_TestTexture : register(t0, space0);

SamplerState u_PointWrap : register(s0);
SamplerState u_PointClamp : register(s1);
SamplerState u_LinearWrap : register(s2);
SamplerState u_LinearClamp : register(s3);
SamplerState u_AnisotropicWrap : register(s4);
SamplerState u_AnisotropicClamp : register(s5);

float4 main(VertexOut pIn) : SV_Target
{
    return u_TestTexture.Sample(u_PointClamp, float2(0,0));
}

struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};


Texture2D u_TestTexture : register(t1, space0);

SamplerState u_PointClamp : register(s2, space0);
SamplerState u_PointWrap : register(s3, space0);


float4 main(VertexOut pIn) : SV_Target
{
    return u_TestTexture.Sample(u_PointWrap, pIn.uv);
}
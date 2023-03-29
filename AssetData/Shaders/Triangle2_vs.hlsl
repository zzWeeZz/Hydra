
struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};


struct VertexIn
{
    float3 Position : POSTION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 Texcoord : TEXCOORD;
};

struct CameraData
{
    float4x4 view;
    float4x4 proj;
    float4x4 modelSpaco;
    float4x4 GetMVP()
    {
        return mul(proj, mul(view, modelSpaco));
    }
};

ConstantBuffer<CameraData> u_CamData : register(b0, space0);

VertexOut main(VertexIn inVertex, uint vertexID : SV_VertexID)
{
    VertexOut outer = (VertexOut) 0;
  
    outer.position = mul(u_CamData.GetMVP(), float4(inVertex.Position, 1.f));
    outer.color = float4(1,1,1,1);
    outer.uv = inVertex.Texcoord;
    
    return outer;
}

struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};


struct VertexIn
{
    float4 Position : POSTION;
    float4 Color : COLOR;
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
  
    outer.position = mul(u_CamData.GetMVP(), inVertex.Position);
    outer.color = inVertex.Color;
    
    return outer;
}
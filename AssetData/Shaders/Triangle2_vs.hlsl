
struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};


struct VertexIn
{
    float4 Position : POSTION;
};

struct CameraData
{
    float4x4 view;
    float4x4 proj;
    float4x4 modelSpaco;
    float4x4 GetModel()
    {
        return modelSpaco;
    }
};

ConstantBuffer<CameraData> u_CamData : register(b0, space0);

VertexOut main(VertexIn inVertex, uint vertexID : SV_VertexID)
{
    VertexOut outer = (VertexOut) 0;
  
    float4 colours[8] =
    {
        float4(1.0f, 0.0f, 0.0f, 1.0f),
        float4(0.0f, 1.0f, 0.0f, 1.0f),
        float4(0.0f, 0.0f, 1.0f, 1.0f),
        float4(1.0f, 0.0f, 1.0f, 1.0f),
        float4(1.0f, 1.0f, 1.0f, 1.0f),
        float4(1.0f, 0.0f, 0.0f, 1.0f),
        float4(0.0f, 1.0f, 0.0f, 1.0f),
        float4(0.0f, 0.0f, 1.0f, 1.0f),
    };
    float4x4 mvp = mul(u_CamData.proj, mul(u_CamData.view, u_CamData.GetModel()));
    outer.position = mul(u_CamData.modelSpaco, inVertex.Position);
    outer.position = mul(u_CamData.view, outer.position);
    outer.position = mul(u_CamData.proj, outer.position);
    outer.color = colours[vertexID];
    
    return outer;
}
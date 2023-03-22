
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
    float4 color;
};

ConstantBuffer<CameraData> u_CamData : register(b0, space0);

VertexOut main(VertexIn inVertex, uint vertexID : SV_VertexID)
{
    VertexOut outer = (VertexOut) 0;
  
    float4 colours[6] =
    {
        float4(1.0f, 0.0f, 0.0f, 1.0f),
        float4(0.0f, 1.0f, 0.0f, 1.0f),
        float4(0.0f, 0.0f, 1.0f, 1.0f),
        float4(1.0f, 0.0f, 1.0f, 1.0f),
        float4(1.0f, 0.0f, 1.0f, 1.0f),
        float4(1.0f, 0.0f, 1.0f, 1.0f)
    };
    
    outer.position =  inVertex.Position;
    outer.color = u_CamData.color;
    
    return outer;
}
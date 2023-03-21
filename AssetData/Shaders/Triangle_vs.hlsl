
struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};


struct VertexIn
{
    float4 Position : POSTION;
};

VertexOut main(VertexIn inVertex, uint vertexID : SV_VertexID)
{
    VertexOut outer = (VertexOut) 0;
    //float4 vertices[3] =
    //{
    //    float4(-1.0f, -1.0f, 0.0f, 1.0f),
    //    float4(0.0f, 1.0f, 0.0f, 1.0f),
    //    float4(1.0f, -1.0f, 0.0f, 1.0f)
    //};
    float4 colours[6] =
    {
        float4(1.0f, 0.0f, 0.0f, 1.0f),
        float4(0.0f, 1.0f, 0.0f, 1.0f),
        float4(0.0f, 0.0f, 1.0f, 1.0f),
        float4(1.0f, 0.0f, 1.0f, 1.0f),
        float4(1.0f, 0.0f, 1.0f, 1.0f),
        float4(1.0f, 0.0f, 1.0f, 1.0f)
    };
    
    outer.position = inVertex.Position;
    outer.color = colours[vertexID];
    
    return outer;
}
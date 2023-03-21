
struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VertexOut main(uint vertexID : SV_VertexID)
{
    VertexOut outer = (VertexOut) 0;
    float4 vertices[3] =
    {
        float4(-1.0f, -1.0f, 0.0f, 1.0f),
        float4(0.0f, 1.0f, 0.0f, 1.0f),
        float4(1.0f, -1.0f, 0.0f, 1.0f)
    };
    float4 colours[3] =
    {
        float4(1.0f, 0.0f, 0.0f, 1.0f),
        float4(0.0f, 1.0f, 0.0f, 1.0f),
        float4(0.0f, 0.0f, 1.0f, 1.0f)
    };
    outer.position = vertices[vertexID];
    outer.color = colours[vertexID];
    
    return outer;
}

struct VertexOut
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

float4 main(VertexOut pIn) : SV_Target
{
    return pIn.color;
}
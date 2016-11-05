#include "common_include.hlsli"

VertexShaderOutput main(AppData app)
{
    VertexShaderOutput output;

    output.position = mul(float4(app.PosL, 1.0f), gWorldViewProj);
    output.positionVS = mul(float4(app.PosL, 1.0f), gWorldView).xyz;
    output.tangentVS = mul(app.Tangent, (float3x3) gWorldView);
    output.binormalVS = mul(app.Bitangent, (float3x3) gWorldView);
    output.normalVS = mul(app.Normal, (float3x3) gWorldView);
    output.texCoord = app.UV;

    return output; 
}
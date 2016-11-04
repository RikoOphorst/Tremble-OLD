#ifndef VERTEXDATAHLSL
#define VERTEXDATAHLSL

struct VertexIn
{
	float3 PosL : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Bitangent : BITANGENT;
	float2 UV : UV;
	float4 Color : COLOR;
};

#endif
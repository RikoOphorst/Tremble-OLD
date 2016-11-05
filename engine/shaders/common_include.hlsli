#ifndef COMMONINCLUDEHLSL
#define COMMONINCLUDEHLSL

struct AppData
{
    float3 PosL : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float2 UV : UV;
    float4 Color : COLOR;
};

struct VertexShaderOutput
{
    float3 positionVS : TEXCOORD0; // view space position
    float2 texCoord : TEXCOORD1; // texture coordinates
    float3 tangentVS : TANGENT; // view space tangent vector
    float3 binormalVS : BINORMAL; // view space binormal vector
    float3 normalVS : NORMAL; // view space normal vector
    float4 position : SV_POSITION; // clip space position
};

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gWorldView;
    float4x4 gWorldViewProj;
}

cbuffer cbPerPass : register(b1)
{
    float total_time;
    float delta_time;
    float near_z;
    float far_z;
    float4x4 view;
    float4x4 inv_view;
    float4x4 projection;
    float4x4 inv_projection;
    float4x4 view_projection;
    float4x4 inv_view_projection;
    float3 eye_pos_w;
    float pass_padding;
    float2 render_target_size;
    float2 inv_render_target_size;
}

Texture2D AmbientTexture : register(t0);
Texture2D EmissiveTexture : register(t1);
Texture2D DiffuseTexture : register(t2);
Texture2D SpecularTexture : register(t3);
Texture2D SpecularPowerTexture : register(t4);
Texture2D NormalTexture : register(t5);
Texture2D BumpTexture : register(t6);
Texture2D OpacityTexture : register(t7);

struct Light
{
    float4 PositionWS;
    //-- 16 bytes
    float4 DirectionWS;
    //-- 16 bytes
    float4 PositionVS;
    //-- 16 bytes
    float4 DirectionVS;
    //-- 16 bytes
    float4 Color;
    //-- 16 bytes
    float SpotlightAngle;
    float Range;
    float Intensity;
    bool Enabled;
    //-- 16 bytes
    bool Selected;
    uint Type;
    float2 Padding;
    //-- 16 bytes
    //-- 16 * 8 = 112 bytes
};

StructuredBuffer<Light> lights : register(t8);

struct Material
{
    float4 GlobalAmbient;
    //-- 16 bytes
    float4 AmbientColor;
    //-- 16 bytes
    float4 EmissiveColor;
    //-- 16 bytes
    float4 DiffuseColor;
    //-- 16 bytes
    float4 SpecularColor;
    //-- 16 bytes
    float4 Reflectance;
    //-- 16 bytes
    float Opacity;
    float SpecularPower;
    float IndexOfRefraction;
    bool HasAmbientTexture;
    //-- 16 bytes
    bool HasEmissiveTexture;
    bool HasDiffuseTexture;
    bool HasSpecularTexture;
    bool HasSpecularPowerTexture;
    //-- 16 bytes
    bool HasNormalTexture;
    bool HasBumpTexture;
    bool HasOpacityTexture;
    float BumpIntensity;
    //-- 16 bytes
    float SpecularScale;
    float AlphaThreshold;
    float2 Padding;
    //-- 16 bytes
    //-- 16 * 10 = 160 bytes
};

cbuffer cbPerMaterial : register(b2)
{
    Material gMaterial;
};

SamplerState samplerPointWrap : register(s0);
SamplerState samplerPointClamp : register(s1);
SamplerState samplerLinearWrap : register(s2);
SamplerState samplerLinearClamp : register(s3);
SamplerState samplerAnisotropicWrap : register(s4);
SamplerState samplerAnisotropicClamp : register(s5);

float3 ExpandNormal(float3 n)
{
    return n * 2.0f - 1.0f;
}

float4 DoNormalMapping(float3x3 TBN, Texture2D tex, SamplerState s, float2 uv)
{
    float3 normal = tex.Sample(s, uv).xyz;
    normal = ExpandNormal(normal);
    normal = mul(normal, TBN);
    return normalize(float4(normal, 0));
}

// still magic
float4 DoBumpMapping(float3x3 TBN, Texture2D tex, SamplerState s, float2 uv, float bumpScale)
{
    // Sample the heightmap at the current texture coordinate.
    float height = tex.Sample(s, uv).r * bumpScale;
    // Sample the heightmap in the U texture coordinate direction.
    float heightU = tex.Sample(s, uv, int2(1, 0)).r * bumpScale;
    // Sample the heightmap in the V texture coordinate direction.
    float heightV = tex.Sample(s, uv, int2(0, 1)).r * bumpScale;
 
    float3 p = { 0, 0, height };
    float3 pU = { 1, 0, heightU };
    float3 pV = { 0, 1, heightV };
 
    // normal = tangent x bitangent
    float3 normal = cross(normalize(pU - p), normalize(pV - p));
 
    // Transform normal from tangent space to view space.
    normal = mul(normal, TBN);
 
    return float4(normal, 0);
}

#endif
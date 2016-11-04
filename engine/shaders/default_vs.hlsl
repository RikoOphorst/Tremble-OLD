#include "cbuffers.hlsli"
#include "samplers.hlsli"
#include "vertex_data.hlsli"
#include "lighting.hlsli"

Texture2D mat_emissive_map : register(t0);
Texture2D mat_ambient_map : register(t1);
Texture2D mat_diffuse_map : register(t2);
Texture2D mat_specular_map : register(t3);
Texture2D mat_shininess_map : register(t4);
Texture2D mat_normal_map : register(t5);

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float4 PosW : TEST;
    float3 Normal : NORMAL;
    float3 Bitangent : BINORMAL;
    float3 Tangent : TANGENT;
    float2 UV : UV;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;

    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.Color = vin.Color;
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.UV = vin.UV;
    vout.Normal = mul(vin.Normal, (float3x3) gWorld);
    vout.Bitangent = mul(vin.Bitangent, (float3x3) gWorld);
    vout.Tangent = mul(vin.Tangent, (float3x3) gWorld);

    return vout;
}

float4 PS(VertexOut pin) : SV_TARGET
{
    float3 position = pin.PosW.xyz;
    float3 normal = normalize(pin.Normal);
    float3 to_eye = normalize(eye_pos_w - pin.PosW.xyz);
	
    Material mat =
    {
        mat_emissive,
		mat_diffuse,
		mat_specular,
		mat_shininess,
		mat_ambient_reflectance
    };

    if (mat_use_diffuse_map)
    {
        float4 col = mat_diffuse_map.Sample(samplerPointWrap, pin.UV);
        clip(col.a - 0.05f);

        mat.diffuse = col.xyz;
    }

    if (mat_use_emissive_map)
    {
        mat.emissive = mat_emissive_map.Sample(samplerPointWrap, pin.UV).xyz;
    }

    if (mat_use_specular_map)
    {
        mat.specular = mat_specular_map.Sample(samplerPointWrap, pin.UV).xyz;
    }

    if (mat_use_shininess_map)
    {
        mat.shininess = mat_shininess_map.Sample(samplerPointWrap, pin.UV).x;
    }

    if (mat_use_ambient_map)
    {
        mat.ambient_reflectance = mat_ambient_map.Sample(samplerPointWrap, pin.UV).x;
    }

    if (mat_use_normal_map)
    {
        normal = (mat_normal_map.Sample(samplerPointWrap, pin.UV) * 2.0f - 1.0f).xyz;
        normal = normalize(normal.x * pin.Tangent + normal.y * pin.Bitangent + normal.z * normal);
    }

    float4 color = float4(ComputeLighting(
		position,
		normal,
		to_eye,
		mat,
		float3(1, 0.6, 0.2),
		active_point_lights,
		point_lights,
		active_directional_lights,
		directional_lights,
		active_spot_lights,
		spot_lights,
		active_dual_cone_spot_lights,
		dual_cone_spot_lights
	), 1.0f);

    return color;
}
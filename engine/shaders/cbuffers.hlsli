#include "lights.hlsli"

#ifndef CBUFFERSHLSL
#define CBUFFERSHLSL

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
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
	float active_point_lights;
	float active_directional_lights;
	float active_spot_lights;
	float active_dual_cone_spot_lights;
	PointLight point_lights[16];
	DirectionalLight directional_lights[16];
	SpotLight spot_lights[16];
	DualConeSpotLight dual_cone_spot_lights[16];
}

cbuffer cbPerMaterial : register(b2)
{
	float3 mat_emissive;
    int mat_use_emissive_map;
    float3 mat_diffuse;
    int mat_use_diffuse_map;
    float3 mat_specular;
    int mat_use_specular_map;
    float3 mat_ambient_reflectance;
    int mat_use_ambient_map;
    float mat_shininess;
    int mat_use_shininess_map;
    int mat_use_normal_map;
    float mat_padding;
}

#endif
#include "lights.hlsli"
#include "material.hlsli"

#ifndef LIGHTINGHLSL
#define LIGHTINGHLSL

float ComputeAttenuation(float d, float falloff_start, float falloff_end)
{
	return saturate((falloff_end - d) / (falloff_end - falloff_start));
}

float3 ComputeBlinnPhong(float3 light_color, float3 to_light, float3 to_eye, float3 normal, float3 ambient, Material material)
{
	float3 half_vec = normalize(to_light + to_eye);
	float spec_factor = pow(max(dot(normal, half_vec), 0.0f), 10.0f);

	float3 spec_term = material.specular * spec_factor;

    return (material.diffuse + spec_term) * light_color;
}

float3 ComputePointLight(PointLight light, float3 ambient, Material mat, float3 position, float3 normal, float3 to_eye)
{
	float3 to_light = light.position - position;
	float d = length(to_light);

	float3 color;

	if (d > light.falloff_end)
	{
		color = float3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		to_light /= d;

		float NdotL = max(dot(to_light, normal), 0.0f);
		float3 light_strength = light.color * NdotL;

		float attenuation = ComputeAttenuation(d, light.falloff_start, light.falloff_end);
		light_strength *= attenuation;

		color = ComputeBlinnPhong(light_strength, to_light, to_eye, normal, ambient, mat);
	}

	return color;
}

float3 ComputeDirectionalLight(DirectionalLight light, float3 ambient, Material mat, float3 normal, float3 to_eye)
{
	float3 to_light = normalize(-light.direction);

	float NdotL = max(dot(to_light, normal), 0.0f);
	float3 light_strength = light.color * NdotL;

	return ComputeBlinnPhong(light_strength, to_light, to_eye, normal, ambient, mat);
}

float3 ComputeSpotLight(SpotLight light, float3 ambient, Material mat, float3 position, float3 normal, float3 to_eye)
{
    float3 light_to_p = normalize(position - light.position);
    float cos_direction = dot(light_to_p, light.direction);

    float3 result;

    if (light.cos_cone_angle <= cos_direction)
    {
        PointLight pl;
        pl.color = light.color;
        pl.falloff_start = light.falloff_start;
        pl.position = light.position;
        pl.falloff_end = light.falloff_end;

        result = ComputePointLight(pl, ambient, mat, position, normal, to_eye);
    }
    else
    {
        result = float3(0.0f, 0.0f, 0.0f);
    }

    return result;
}

float3 ComputeDualConeSpotLight(DualConeSpotLight light, float3 ambient, Material mat, float3 position, float3 normal, float3 to_eye)
{
    float3 light_to_p = normalize(position - light.position);
    float cos_direction = dot(light_to_p, light.direction);

    float3 result;

    if (light.cos_cone_outer_angle <= cos_direction)
    {
        PointLight pl;
        pl.color = light.color;
        pl.falloff_start = light.falloff_start;
        pl.position = light.position;
        pl.falloff_end = light.falloff_end;

        result = smoothstep(light.cos_cone_outer_angle, light.cos_cone_inner_angle, cos_direction) * ComputePointLight(pl, ambient, mat, position, normal, to_eye);;
    }
    else
    {
        result = float3(0.0f, 0.0f, 0.0f);
    }

    return result;
}

float3 ComputeLighting(
	float3 position,
	float3 normal,
	float3 to_eye,
	Material mat,
	float3 ambient,
	float active_point_lights,
	PointLight point_lights[16],
	float active_directional_lights,
	DirectionalLight directional_lights[16],
	float active_spot_lights,
	SpotLight spot_lights[16],
	float active_dual_cone_spot_lights,
	DualConeSpotLight dual_cone_spot_lights[16]
)
{
    float3 color = mat.emissive + (mat.diffuse * ambient * 0.1f);

	for (int i = 0; i < active_point_lights; i++)
	{
		color += ComputePointLight(point_lights[i], ambient, mat, position, normal, to_eye);
    }

    for (int j = 0; j < active_directional_lights; j++)
    {
        color += ComputeDirectionalLight(directional_lights[j], ambient, mat, normal, to_eye);
    }

    for (int k = 0; k < active_spot_lights; k++)
    {
        color += ComputeSpotLight(spot_lights[k], ambient, mat, position, normal, to_eye);
    }

    for (int l = 0; l < active_dual_cone_spot_lights; l++)
    {
        color += ComputeDualConeSpotLight(dual_cone_spot_lights[l], ambient, mat, position, normal, to_eye);
    }

	return clamp(color, float3(0.0f, 0.0f, 0.0f), float3(1.0f, 1.0f, 1.0f));
}
#endif
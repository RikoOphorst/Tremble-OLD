#include "common_include.hlsli"

#define NUM_LIGHTS 64
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
};

float4 DoDiffuse(Light light, float4 L, float4 N)
{
    return light.Color * max(dot(N, L), 0);
}

float4 DoSpecular(Light light, Material material, float4 V, float4 L, float4 N)
{
    float4 R = normalize(reflect(-L, N));
    float RdotV = max(dot(R, V), 0);

    return light.Color * pow(RdotV, material.SpecularPower);
}

float DoAttenuation(Light light, float d)
{
    return 1.0f - smoothstep(light.Range * 0.75f, light.Range, d);
}

LightingResult DoPointLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
    LightingResult result;

    float4 L = light.PositionVS - P;
    float dist = length(L);
    L = L / dist;

    float attenuation = DoAttenuation(light, dist);
    
    result.Diffuse = DoDiffuse(light, L, N) * attenuation * light.Intensity;
    result.Specular = DoSpecular(light, mat, V, L, N) * attenuation * light.Intensity;

    return result;
}

float DoSpotCone(Light light, float4 L)
{
    // If the cosine angle of the light's direction 
    // vector and the vector from the light source to the point being 
    // shaded is less than minCos, then the spotlight contribution will be 0.
    float minCos = cos(radians(light.SpotlightAngle));
    // If the cosine angle of the light's direction vector
    // and the vector from the light source to the point being shaded
    // is greater than maxCos, then the spotlight contribution will be 1.
    float maxCos = lerp(minCos, 1, 0.5f);
    float cosAngle = dot(light.DirectionVS, -L);
    // Blend between the minimum and maximum cosine angles.
    return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoSpotLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
    LightingResult result;
 
    float4 L = light.PositionVS - P;
    float distance = length(L);
    L = L / distance;
 
    float attenuation = DoAttenuation(light, distance);
    float spotIntensity = DoSpotCone(light, L);
 
    result.Diffuse = DoDiffuse(light, L, N) *
                       attenuation * spotIntensity * light.Intensity;
    result.Specular = DoSpecular(light, mat, V, L, N) *
                       attenuation * spotIntensity * light.Intensity;
 
    return result;
}

LightingResult DoDirectionalLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
    LightingResult result;
 
    float4 L = normalize(-light.DirectionVS);
 
    result.Diffuse = DoDiffuse(light, L, N) * light.Intensity;
    result.Specular = DoSpecular(light, mat, V, L, N) * light.Intensity;
 
    return result;
}

LightingResult DoLighting(StructuredBuffer<Light> lights, Material mat, float4 eye_pos, float4 P, float4 N)
{
    float4 V = normalize(eye_pos - P);

    LightingResult totalResult = (LightingResult)0;

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        LightingResult result = (LightingResult)0;

        if (!lights[i].Enabled)
            continue;

        if (lights[i].Type != DIRECTIONAL_LIGHT && length(lights[i].PositionVS - P) > lights[i].Range)
            continue;

        switch (lights[i].Type)
        {
        case DIRECTIONAL_LIGHT:
            result = DoDirectionalLight(lights[i], mat, V, P, N);
            break;
        case POINT_LIGHT:
            result = DoPointLight(lights[i], mat, V, P, N);
            break;
        case SPOT_LIGHT:
            result = DoSpotLight(lights[i], mat, V, P, N);
            break;
        }

        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    return totalResult;
}

[earlydepthstencil]
float4 main(VertexShaderOutput input) : SV_TARGET
{
    // everything in view space
    float4 eye_pos = float4(0.0f, 0.0f, 0.0f, 1.0f);

    Material mat = gMaterial;

    float4 diffuse = mat.DiffuseColor;
    if (mat.HasDiffuseTexture)
    {
        float4 diffuseTex = DiffuseTexture.Sample(samplerLinearWrap, input.texCoord);
        if (any(diffuse.rgb))
        {
            diffuse *= diffuseTex;
        }
        else
        {
            diffuse = diffuseTex;
        }
    }

    float alpha = diffuse.a;
    if (mat.HasOpacityTexture)
    {
        alpha = OpacityTexture.Sample(samplerLinearWrap, input.texCoord).r;
    }

	clip(alpha - mat.AlphaThreshold);

    float4 ambient = mat.AmbientColor;
    if (mat.HasAmbientTexture)
    {
        float4 ambientTex = AmbientTexture.Sample(samplerLinearWrap, input.texCoord);
        if (any(ambient.rgb))
        {
            ambient *= ambientTex;
        }
        else
        {
            ambient = ambientTex;
        }
    }
    ambient *= mat.GlobalAmbient;

    float4 emissive = mat.EmissiveColor;
    if (mat.HasEmissiveTexture)
    {
        float4 emissiveTex = EmissiveTexture.Sample(samplerLinearWrap, input.texCoord);
        if (any(emissive.rgb))
        {
            emissive *= emissiveTex;
        }
        else
        {
            emissive = emissiveTex;
        }
    }

    if (mat.HasSpecularPowerTexture)
    {
        mat.SpecularPower = SpecularPowerTexture.Sample(samplerLinearWrap, input.texCoord).r * mat.SpecularScale;
    }

    float4 N = float4(0.0f, 0.0f, 0.0f, 0.0f);
    if (mat.HasNormalTexture)
    {
        // For scenes with normal mapping, I don't have to invert the binormal.
        float3x3 TBN = float3x3(
                             normalize(input.tangentVS),
                             normalize(input.binormalVS),
                             normalize(input.normalVS));
 
        N = DoNormalMapping(TBN, NormalTexture, samplerLinearWrap, input.texCoord);
    }
    else if (mat.HasBumpTexture)
    {
        // For most scenes using bump mapping, I have to invert the binormal.
        float3x3 TBN = float3x3(normalize(input.tangentVS),
                             normalize(-input.binormalVS),
                             normalize(input.normalVS));
 
        N = DoBumpMapping(TBN, BumpTexture, samplerLinearWrap, input.texCoord, mat.BumpIntensity);
    }
    else
    {
        N = normalize(float4(input.normalVS, 0));
    }

    float4 P = float4(input.positionVS, 1);

    LightingResult lit = DoLighting(lights, mat, eye_pos, P, N);

    diffuse *= float4(lit.Diffuse.rgb, 1.0f); // discard alpha, we roll our own

    float4 specular = 0;
    if (mat.SpecularPower > 1.0f)
    {
        specular = mat.SpecularColor;
        if (mat.HasSpecularTexture)
        {
            float4 specularTex = SpecularTexture.Sample(samplerLinearWrap, input.texCoord);
            if (any(specular.rgb))
            {
                specular *= specularTex;
            }
            else
            {
                specular = specularTex;
            }
        }
        specular *= lit.Specular;
    }

    return float4( (ambient + emissive + diffuse + specular).rgb, alpha * mat.Opacity );
}
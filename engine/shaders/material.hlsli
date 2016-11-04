#ifndef MATERIALHLSL
#define MATERIALHLSL

struct Material
{
	float3 emissive;
	float3 diffuse;
	float3 specular;
	float shininess;
	float3 ambient_reflectance;
};

#endif
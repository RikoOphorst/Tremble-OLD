#pragma once

#include "../../pch.h"
#include "../../components/rendering/light.h"

namespace engine
{
	struct DebugConstants
	{
		DirectX::XMMATRIX world_view_projection = DirectX::XMMatrixIdentity();
	};

	struct ObjectConstants
	{
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX world_view_projection = DirectX::XMMatrixIdentity();
	};

	struct PassConstants
	{
		float total_time;
		float delta_time;
		float near_z;
		float far_z;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX inv_view;
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX inv_projection;
		DirectX::XMMATRIX view_projection;
		DirectX::XMMATRIX inv_view_projection;
		DirectX::XMFLOAT3 eye_pos_world;
		float padding;
		DirectX::XMFLOAT2 render_target_size;
		DirectX::XMFLOAT2 inv_render_target_size;
		float active_point_lights;
		float active_directional_lights;
		float active_spot_lights;
		float active_dual_cone_spot_lights;
		PointLight point_lights[16];
		DirectionalLight directional_lights[16];
		SpotLight spot_lights[16];
		DualConeSpotLight dual_cone_spot_lights[16];
	};

	struct MaterialConstants
	{
		DirectX::XMFLOAT3 emissive;
		int use_emissive_map;
		DirectX::XMFLOAT3 diffuse;
		int use_diffuse_map;
		DirectX::XMFLOAT3 specular;
		int use_specular_map;
		DirectX::XMFLOAT3 ambient_reflectance;
		int use_ambient_map;
		float shininess;
		int use_shininess_map;
		int use_normal_map;
		float padding;
	};
}
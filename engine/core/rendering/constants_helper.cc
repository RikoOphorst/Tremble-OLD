#include "pch.h"
#include "constants_helper.h"

#include "../../components/rendering/renderable.h"
#include "../../components/rendering/camera.h"
#include "../../components/rendering/light.h"
#include "../scene_graph/scene_graph.h"
#include "upload_buffer.h"
#include "material.h"

namespace engine
{
	void ConstantsHelper::UpdatePassConstants(
		UploadBuffer<PassConstants>* buffer,
		const DirectX::XMFLOAT2& render_target_size,
		Timer* timer,
		Camera* camera
	)
	{
		PassConstants constants;
		constants.delta_time = static_cast<float>(timer->GetDeltaT());
		constants.total_time = static_cast<float>(timer->GetTimeSinceStartup());
		constants.far_z = camera->GetFarZ();
		constants.near_z = camera->GetNearZ();
		constants.render_target_size = render_target_size;
		constants.inv_render_target_size = DirectX::XMFLOAT2(1.0f / render_target_size.x, 1.0f / render_target_size.y);
		constants.view = camera->GetView();
		constants.inv_view = camera->GetInvView();
		constants.projection = camera->GetProjection();
		constants.inv_projection = camera->GetInvProjection();
		constants.view_projection = camera->GetViewProjection();
		constants.inv_view_projection = camera->GetInvViewProjection();
		DirectX::XMStoreFloat3(&constants.eye_pos_world, camera->GetNode()->GetPosition());

		buffer->CopyData(0, constants);
	}
	
	//------------------------------------------------------------------------------------------------------
	void ConstantsHelper::UpdateObjectConstants(UploadBuffer<ObjectConstants>* buffer, const std::vector<Renderable*>& renderables, Camera* camera)
	{
		ObjectConstants constants;

		for (int i = 0; i < static_cast<int>(renderables.size()); i++)
		{
			constants.world = renderables[i]->GetNode()->GetWorldTransform();
			constants.world_view = constants.world * camera->GetView();
			constants.world_view_projection = constants.world * camera->GetViewProjection();

			buffer->CopyData(renderables[i]->GetRendererObjectCBId(), constants);
		}
	}
	
	//------------------------------------------------------------------------------------------------------
	void ConstantsHelper::UpdateMaterialConstants(UploadBuffer<MaterialConstants>* buffer, const std::vector<Material*>& materials)
	{
		MaterialConstants constants;

		for (int i = 0; i < static_cast<int>(materials.size()); i++)
		{
			constants.global_ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
			constants.ambient_color = DirectX::XMFLOAT4(materials[i]->ambient_reflectance.x, materials[i]->ambient_reflectance.y, materials[i]->ambient_reflectance.z, 1.0f);
			constants.emissive_color = DirectX::XMFLOAT4(materials[i]->emissive.x, materials[i]->emissive.y, materials[i]->emissive.z, 1.0f);
			constants.diffuse_color = DirectX::XMFLOAT4(materials[i]->diffuse.x, materials[i]->diffuse.y, materials[i]->diffuse.z, 1.0f);
			constants.specular_color = DirectX::XMFLOAT4(materials[i]->specular.x, materials[i]->specular.y, materials[i]->specular.z, 1.0f);
			constants.reflectance = DirectX::XMFLOAT4(materials[i]->ambient_reflectance.x, materials[i]->ambient_reflectance.y, materials[i]->ambient_reflectance.z, 1.0f);
			constants.opacity = 1.0f;
			constants.specular_power = 5.0f;
			constants.index_of_refraction = 1.0f;
			constants.has_ambient_texture = materials[i]->use_ambient_map;
			constants.has_emissive_texture = materials[i]->use_emissive_map;
			constants.has_diffuse_texture = materials[i]->use_diffuse_map;
			constants.has_specular_texture = materials[i]->use_specular_map;
			constants.has_specular_power_texture = false;
			constants.has_normal_texture = materials[i]->use_normal_map;
			constants.has_bump_texture = false;
			constants.has_opacity_texture = false;
			constants.bump_intensity = 1.0f;
			constants.specular_scale = 1.0f;
			constants.alpha_threshold = 0.05f;
			constants.padding[0] = constants.padding[1] = 0.0f;

			buffer->CopyData(materials[i]->renderer_material_cb_id, constants);
		}
	}
}
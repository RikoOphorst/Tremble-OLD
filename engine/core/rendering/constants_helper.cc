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
		Camera* camera,
		const std::vector<Light*>& lights
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

		int point_lights = 0;
		int directional_lights = 0;
		int spot_lights = 0;
		int dual_cone_spot_lights = 0;

		for (int i = 0; i < static_cast<int>(lights.size()); i++)
		{
			switch (lights[i]->GetLightType())
			{
			case LightTypePoint:
				constants.point_lights[point_lights].color = lights[i]->GetColor();
				constants.point_lights[point_lights].falloff_start = lights[i]->GetFalloffStart();
				constants.point_lights[point_lights].falloff_end = lights[i]->GetFalloffEnd();
				constants.point_lights[point_lights].position = lights[i]->GetNode()->GetPosition();
				constants.active_point_lights = static_cast<float>(++point_lights);
				break;

			case LightTypeDirectional:
				constants.directional_lights[directional_lights].color = lights[i]->GetColor();
				constants.directional_lights[directional_lights].direction = lights[i]->GetDirection();
				constants.active_directional_lights = static_cast<float>(++directional_lights);
				break;

			case LightTypeSpot:
				constants.spot_lights[spot_lights].color = lights[i]->GetColor();
				constants.spot_lights[spot_lights].direction = lights[i]->GetDirection();
				constants.spot_lights[spot_lights].falloff_start = lights[i]->GetFalloffStart();
				constants.spot_lights[spot_lights].falloff_end = lights[i]->GetFalloffEnd();
				constants.spot_lights[spot_lights].cos_cone_angle = lights[i]->GetCosCone();
				constants.spot_lights[spot_lights].position = lights[i]->GetNode()->GetPosition();
				constants.active_spot_lights = static_cast<float>(++spot_lights);
				break;

			case LightTypeDualConeSpot:
				constants.dual_cone_spot_lights[dual_cone_spot_lights].color = lights[i]->GetColor();
				constants.dual_cone_spot_lights[dual_cone_spot_lights].direction = lights[i]->GetDirection();
				constants.dual_cone_spot_lights[dual_cone_spot_lights].falloff_start = lights[i]->GetFalloffStart();
				constants.dual_cone_spot_lights[dual_cone_spot_lights].falloff_end = lights[i]->GetFalloffEnd();
				constants.dual_cone_spot_lights[dual_cone_spot_lights].cos_cone_inner_angle = lights[i]->GetCosInnerCone();
				constants.dual_cone_spot_lights[dual_cone_spot_lights].cos_cone_outer_angle = lights[i]->GetCosOuterCone();
				constants.dual_cone_spot_lights[dual_cone_spot_lights].position = lights[i]->GetNode()->GetPosition();
				constants.active_dual_cone_spot_lights = static_cast<float>(++dual_cone_spot_lights);
				break;
			}
		}

		buffer->CopyData(0, constants);
	}
	
	//------------------------------------------------------------------------------------------------------
	void ConstantsHelper::UpdateObjectConstants(UploadBuffer<ObjectConstants>* buffer, const std::vector<Renderable*>& renderables, Camera* camera)
	{
		ObjectConstants constants;

		for (int i = 0; i < static_cast<int>(renderables.size()); i++)
		{
			constants.world = renderables[i]->GetNode()->GetWorldTransform();
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
			constants.emissive = materials[i]->emissive;
			constants.use_emissive_map = materials[i]->use_emissive_map;
			constants.diffuse = materials[i]->diffuse;
			constants.use_diffuse_map = materials[i]->use_diffuse_map;
			constants.specular = materials[i]->specular;
			constants.use_specular_map = materials[i]->use_specular_map;
			constants.ambient_reflectance = materials[i]->ambient_reflectance;
			constants.use_ambient_map = materials[i]->use_ambient_map;
			constants.use_normal_map = materials[i]->use_normal_map;
			constants.shininess = materials[i]->shininess;
			constants.use_shininess_map = materials[i]->use_shininess_map;

			buffer->CopyData(materials[i]->renderer_material_cb_id, constants);
		}
	}
}
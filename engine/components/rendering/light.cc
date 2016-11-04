#include "pch.h"
#include "light.h"

#include "../../core/scene_graph/scene_graph.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	Light::Light() :
		direction_(-1.0f, -1.0f, -1.0f),
		color_(1.0f, 1.0f, 1.0f, 1.0f),
		falloff_start_(0.0f),
		falloff_end_(0.0f),
		cos_inner_cone_(0.0f),
		cos_outer_cone_(0.0f),
		cos_cone_(0.0f)
	{

	}

	//------------------------------------------------------------------------------------------------------
	void Light::Awake()
	{
		Get::Renderer()->AddLight(this);
	}

	//------------------------------------------------------------------------------------------------------
	void Light::Update()
	{
		switch (light_type_)
		{
		case LightTypePoint:
			point_light_.position = GetNode()->GetPosition();
			point_light_.color = color_;
			point_light_.falloff_start = falloff_start_;
			point_light_.falloff_end = falloff_end_;
			break;
		case LightTypeDirectional:
			directional_light_.direction = direction_;
			directional_light_.color = color_;
			break;
		case LightTypeSpot:
			spot_light_.position = GetNode()->GetPosition();
			spot_light_.color = color_;
			spot_light_.direction = direction_;
			spot_light_.falloff_start = falloff_start_;
			spot_light_.falloff_end = falloff_end_;
			spot_light_.cos_cone_angle = cos_cone_;
			break;
		case LightTypeDualConeSpot:
			dual_cone_spot_light_.position = GetNode()->GetPosition();
			dual_cone_spot_light_.color = color_;
			dual_cone_spot_light_.direction = direction_;
			dual_cone_spot_light_.falloff_start = falloff_start_;
			dual_cone_spot_light_.falloff_end = falloff_end_;
			dual_cone_spot_light_.cos_cone_inner_angle = cos_inner_cone_;
			dual_cone_spot_light_.cos_cone_outer_angle = cos_outer_cone_;
			break;
		}
	}
	
	//------------------------------------------------------------------------------------------------------
	const PointLight& Light::GetPointLight()
	{
		assert(light_type_ == LightTypePoint);
		return point_light_;
	}

	//------------------------------------------------------------------------------------------------------
	const DirectionalLight& Light::GetDirectionalLight()
	{
		assert(light_type_ == LightTypeDirectional);
		return directional_light_;
	}

	//------------------------------------------------------------------------------------------------------
	const SpotLight& Light::GetSpotLight()
	{
		assert(light_type_ == LightTypeSpot);
		return spot_light_;
	}

	//------------------------------------------------------------------------------------------------------
	const DualConeSpotLight& Light::GetDualConeSpotLight()
	{
		assert(light_type_ == LightTypeDualConeSpot);
		return dual_cone_spot_light_;
	}
}
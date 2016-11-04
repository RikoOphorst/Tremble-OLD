#pragma once

#include "../../core/math/math.h"
#include "../../core/scene_graph/component.h"

namespace engine
{
	struct PointLight
	{
		DirectX::XMFLOAT3 color;
		float falloff_start;
		DirectX::XMFLOAT3 position;
		float falloff_end;
	}; // size = 32 bytes

	struct DirectionalLight
	{
		DirectX::XMFLOAT3 direction;
		float padding1;
		DirectX::XMFLOAT3 color;
		float padding2;
	}; // size = 32 bytes

	struct SpotLight
	{
		DirectX::XMFLOAT3 color;
		float cos_cone_angle;
		DirectX::XMFLOAT3 position;
		float falloff_start;
		DirectX::XMFLOAT3 direction;
		float falloff_end;
	}; // size = 48 bytes

	struct DualConeSpotLight
	{
		DirectX::XMFLOAT3 color;
		float cos_cone_inner_angle;
		DirectX::XMFLOAT3 position;
		float cos_cone_outer_angle;
		DirectX::XMFLOAT3 direction;
		float falloff_start;
		float falloff_end;
		float padding[3];
	}; // size = 64 bytes

	enum LightType
	{
		LightTypePoint,
		LightTypeDirectional,
		LightTypeSpot,
		LightTypeDualConeSpot
	};

	class Light : public Component
	{
	public:
		Light();

		void Awake();
		void Update();

		void SetLightType(const LightType& light_type) { light_type_ = light_type; }
		void SetDirection(const Vector3& direction) { direction_ = direction; }
		void SetColor(const Vector4& color) { color_ = color; }
		void SetFalloffStart(const Scalar& start) { falloff_start_ = start; }
		void SetFalloffEnd(const Scalar& end) { falloff_end_ = end; }
		void SetInnerCone(const Scalar& inner_cone_angle) { cos_inner_cone_ = Scalar(std::cos(inner_cone_angle)); }
		void SetOuterCone(const Scalar& outer_cone_angle) { cos_outer_cone_ = Scalar(std::cos(outer_cone_angle)); }
		void SetCone(const Scalar& cone_angle) { cos_cone_ = Scalar(std::cos(cone_angle)); }
		void SetCosCone(const Scalar& cone_angle) { cos_cone_ = Scalar(cone_angle); }

		const LightType& GetLightType() const { return light_type_; }
		const Vector3& GetDirection() const { return direction_; }
		const Vector4& GetColor() const { return color_; }
		const Scalar& GetFalloffStart() const { return falloff_start_; }
		const Scalar& GetFalloffEnd() const { return falloff_end_; }
		const Scalar& GetInnerCone() const { return Scalar(std::acos(cos_inner_cone_)); }
		const Scalar& GetOuterCone() const { return Scalar(std::acos(cos_outer_cone_)); }
		const Scalar& GetCosInnerCone() const { return cos_inner_cone_; }
		const Scalar& GetCosOuterCone() const { return cos_outer_cone_; }
		const Scalar& GetCone() const { return Scalar(std::acos(cos_cone_)); }
		const Scalar& GetCosCone() const { return cos_cone_; }

		const PointLight& GetPointLight();
		const DirectionalLight& GetDirectionalLight();
		const SpotLight& GetSpotLight();
		const DualConeSpotLight& GetDualConeSpotLight();
	private:
		LightType light_type_;
		Vector3 direction_;
		Vector4 color_;
		Scalar falloff_start_;
		Scalar falloff_end_;
		Scalar cos_inner_cone_;
		Scalar cos_outer_cone_;
		Scalar cos_cone_;

		PointLight point_light_;
		DirectionalLight directional_light_;
		SpotLight spot_light_;
		DualConeSpotLight dual_cone_spot_light_;
	};
}
#pragma once

#include "../../core/scene_graph/component.h"

namespace engine
{
	class StaticPlane : public Component
	{
	public:
		void Awake();
		Vector3 angle_ = Vector3(0, 1, 0);
		Scalar offset_ = Scalar(0.0f);
	private:
		physx::PxRigidStatic* px_body_;
	};
}
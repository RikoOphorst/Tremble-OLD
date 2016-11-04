#include "pch.h"
#include "plane.h"
#include "../../core/get.h"
#include "../../core/physics/physics_manager.h"

namespace engine
{
	using namespace physx;

	void StaticPlane::Awake()
	{
		px_body_ = PxCreatePlane(*Get::PhysicsManager()->px_physics_, PxPlane(angle_.GetX(), angle_.GetY(), angle_.GetZ(), offset_), *Get::PhysicsManager()->GetDefaultMaterial());
		Get::PhysicsManager()->px_scene_->addActor(*px_body_);
	}
}
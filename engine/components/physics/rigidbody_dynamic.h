#pragma once

#include "physics_rigidbody_component.h"

namespace engine
{
	class RigidbodyDynamic : public PhysicsRigidbodyComponent
	{
	public:
		void Awake();
		void UpdateBeforePhysics();
		void UpdateAfterPhysics();
		void AddImpulseAtPos(Vector3 force, Vector3 world_pos);
		void AddLocalImpulseAtLocalPos(Vector3 force, Vector3 local_pos);
		physx::PxRigidDynamic* GetPxBody() { return px_rigidbody_dynamic_; }
		physx::PxRigidActor* GetPxRigidbodyActor() override;
	private:
		physx::PxRigidDynamic* px_rigidbody_dynamic_;
	};
}
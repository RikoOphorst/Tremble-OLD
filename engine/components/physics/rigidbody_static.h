#pragma once

#include "physics_rigidbody_component.h"

namespace engine
{
	class RigidbodyStatic : public PhysicsRigidbodyComponent
	{
	public:
		physx::PxRigidActor* GetPxRigidbodyActor() override;
		void Awake(const Vector3& dimensions, const Vector3& rotaiton = Vector3(), const Vector3& offset = Vector3(), physx::PxMaterial* material = nullptr);
	private:
		physx::PxRigidStatic* px_rigidbody_static_;
		physx::PxShape* px_shape_;
	};
}
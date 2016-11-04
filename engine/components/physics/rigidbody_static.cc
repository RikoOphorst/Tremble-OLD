#include "pch.h"
#include "rigidbody_static.h"

#include "../../core/get.h"
#include "../../core/scene_graph/scene_graph.h"
#include "../../core/physics/physics_manager.h"

namespace engine
{
	using namespace physx;

	physx::PxRigidActor * RigidbodyStatic::GetPxRigidbodyActor()
	{
		return (PxRigidActor*)px_rigidbody_static_;
	}

	void RigidbodyStatic::Awake(const Vector3& dimensions, const Vector3& rotation, const Vector3& offset, PxMaterial* material)
	{
		ASSERT(GetNode()->IsStatic() == true);

		if (material == nullptr)
		{
			material = Get::PhysicsManager()->GetDefaultMaterial();
		}

		PxShape* box_shape = Get::PhysicsManager()->px_physics_->createShape(PxBoxGeometry(dimensions.ToPxVec3()), *material);
		PxTransform transform(
			DirectX::XMVectorGetX(GetNode()->GetPosition()) + offset.GetX(),
			DirectX::XMVectorGetY(GetNode()->GetPosition()) + offset.GetY(),
			DirectX::XMVectorGetZ(GetNode()->GetPosition()) + offset.GetZ()
			,
			Quaternion(GetNode()->GetRotationQuaternion().GetZ() + rotation.GetZ(), GetNode()->GetRotationQuaternion().GetX() + rotation.GetX(), GetNode()->GetRotationQuaternion().GetY() + rotation.GetY()).ToPxQuat());
		px_rigidbody_static_ = Get::PhysicsManager()->px_physics_->createRigidStatic(transform);
		px_rigidbody_static_->attachShape(*box_shape);
		Get::PhysicsManager()->px_scene_->addActor(*px_rigidbody_static_);
		box_shape->release();
		AddToPhysicsComponents();
	}
}
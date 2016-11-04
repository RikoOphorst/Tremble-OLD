#include "rigidbody_dynamic.h"

#include "../../core/get.h"
#include "../../core/scene_graph/scene_graph.h"
#include "../../core/physics/physics_manager.h"

namespace engine
{
	using namespace physx;

	void RigidbodyDynamic::Awake()
	{
		PxTransform transform(
			DirectX::XMVectorGetX(GetNode()->GetPosition()),
			DirectX::XMVectorGetY(GetNode()->GetPosition()),
			DirectX::XMVectorGetZ(GetNode()->GetPosition())
		,
		Quaternion(GetNode()->GetRotationQuaternion().GetZ(), GetNode()->GetRotationQuaternion().GetX(), GetNode()->GetRotationQuaternion().GetY()).ToPxQuat());
		px_rigidbody_dynamic_ = Get::PhysicsManager()->px_physics_->createRigidDynamic(transform);
		PxShape* shape = Get::PhysicsManager()->px_physics_->createShape(PxBoxGeometry((GetNode()->GetScale() / 2.0f).ToPxVec3()), *Get::PhysicsManager()->GetDefaultMaterial());
		px_rigidbody_dynamic_->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*px_rigidbody_dynamic_, 10.0f);
		Get::PhysicsManager()->px_scene_->addActor(*px_rigidbody_dynamic_);
		AddToPhysicsComponents();
	}

	void RigidbodyDynamic::UpdateBeforePhysics()
	{
		//px_rigidbody_dynamic_->setGlobalPose(
		//	PxTransform(
		//		PxVec3(
		//			GetNode()->GetPosition().GetX(),
		//			GetNode()->GetPosition().GetY(),
		//			GetNode()->GetPosition().GetZ()
		//		),
		//		GetNode()->GetRotation().ToPxQuat()
		//	));
	}

	void RigidbodyDynamic::UpdateAfterPhysics()
	{
		PxTransform get_transform(px_rigidbody_dynamic_->getGlobalPose());
		GetNode()->SetPosition(Vector3(
			get_transform.p.x,
			get_transform.p.y,
			get_transform.p.z
		));
		GetNode()->SetRotationQuaternion(
			Quaternion(get_transform.q)
			);
		//Get::Renderer()->RenderDebugVolume(DebugVolume::CreateCube(GetNode()->GetPosition(), Vector3(1.0f, 1.0f, 1.0f), Vector4(0.8f, 0.8f, 0.8f, 1.0f)));
	}

	void RigidbodyDynamic::AddImpulseAtPos(Vector3 force, Vector3 world_pos)
	{
		PxRigidBodyExt::addForceAtPos(*px_rigidbody_dynamic_, force.ToPxVec3(), world_pos.ToPxVec3(), physx::PxForceMode::eIMPULSE);
	}

	void RigidbodyDynamic::AddLocalImpulseAtLocalPos(Vector3 force, Vector3 local_pos)
	{
		PxRigidBodyExt::addLocalForceAtLocalPos(*px_rigidbody_dynamic_, force.ToPxVec3(), local_pos.ToPxVec3(), physx::PxForceMode::eIMPULSE);
	}

	physx::PxRigidActor* RigidbodyDynamic::GetPxRigidbodyActor()
	{
		return (PxRigidActor*)px_rigidbody_dynamic_;
	}
}
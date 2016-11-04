#include "pch.h"
#include "character_controller.h"
#include "../../core/get.h"
#include "../../core/physics/physics_manager.h"

namespace engine
{
	using namespace physx;

	//------------------------------------------------------------------------------------------------------
	void CharacterController::Awake()
	{
		gravity_ = Get::PhysicsManager()->GetGravity().GetY();
		PxCapsuleControllerDesc desc;
		desc.height = 1.0f;
		desc.radius = 0.75f;

		desc.position = PxExtendedVec3(GetPosition().GetX(), GetPosition().GetY(), GetPosition().GetZ());
		
		desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
		desc.slopeLimit = cos(
			(Scalar(40.0f).ToRadians()));


		PhysicsManager* manager = Get::PhysicsManager();

		desc.material = manager->GetDefaultMaterial();
		if (desc.isValid())
		{
			px_controller_ = manager->px_controller_manager_->createController(desc);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void CharacterController::Move(const Vector3& movement)
	{
		movements_ += movement;
	}

	//------------------------------------------------------------------------------------------------------
	void CharacterController::Jump(Scalar velocity)
	{
		vertical_velocity_ += velocity;
	}

	//------------------------------------------------------------------------------------------------------
	bool CharacterController::IsOnGround()
	{
		return is_on_ground_;
	}

	//------------------------------------------------------------------------------------------------------
	void CharacterController::Update()
	{
		vertical_velocity_ += Scalar(gravity_ * Get::DeltaT());
		movements_.SetY(movements_.GetY() + vertical_velocity_);
		PxControllerCollisionFlags flags = px_controller_->move((movements_ * Get::DeltaT()).ToPxVec3(), 0, Get::DeltaT(), PxControllerFilters());
		if (flags == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			vertical_velocity_ = Scalar(0);
			is_on_ground_ = true;
		}
		else
		{
			is_on_ground_ = false;
		}
		SetPosition(Vector3(px_controller_->getPosition()));
		movements_ = Vector3(0, 0, 0);
	}
}
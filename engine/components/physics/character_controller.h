#pragma once

#include <PxPhysicsAPI.h>
#include "../../core/scene_graph/component.h"

namespace engine
{
	class CharacterController : public Component
	{
	public:
		void Awake(); //!< Creates the character controller
		void Update(); //!< Automatically adds gravity to the made moves, and moves the character
		void Move(const Vector3& movement); //!< Move the character (doesn't actually move it. The movement itself happens in the update). No need to specify gravity
		void Jump(Scalar velocity); //!< Make the character jump in the air(Doesn't check whether it is on the ground)
		void SetGravity(float gravity) { gravity_ = gravity; } //!< Set the gravity of this character
		bool IsOnGround();
	private:
		bool is_on_ground_; //!< Is the character standing on the ground?
		float gravity_;
		Scalar vertical_velocity_; //!< Vertical velocity of this character
		Vector3 movements_; //!< Movements, piled up during the update
		physx::PxController* px_controller_; //!< Pointer to physx character controller
	};
}
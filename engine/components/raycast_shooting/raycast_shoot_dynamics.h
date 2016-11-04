#pragma once

#include "pch.h"
#include "../engine/core/scene_graph/component.h"
#include "../engine/core/scene_graph/scene_graph.h"
#include "../engine/core/physics/physics_manager.h"
#include "../physics/rigidbody_dynamic.h"
#include "../engine/core/input/input_manager.h"

namespace engine
{
	class RaycastShootDynamics : public Component
	{
	public:
		float power = 2.0f;

		void ShootTargets(Vector3 start_position, Vector3 direction)
		{
			physx::PxRaycastBuffer raycast_buffer;
			if (Get::PhysicsManager()->Raycast(&raycast_buffer, start_position, direction, 1000.0f))
			{
				Component* component = Get::PhysicsManager()->GetComponentFromRaycast(&raycast_buffer);
				if (component != nullptr)
				{
					if (component->GetType() == typeid(RigidbodyDynamic))
					{
						static_cast<RigidbodyDynamic*>(component)->AddImpulseAtPos(direction * power, raycast_buffer.block.position);
					}
				}
			}
		}
	private:
		SGNode* camera_node_;
	};
}
#include "component_manager.h"
#include "../../components/physics/physics_rigidbody_component.h"

namespace engine
{
	void ComponentManager::AddPxActorComponent(PhysicsRigidbodyComponent* component)
	{
		physics_components_[component->GetPxRigidbodyActor()] = component;
	}

	void ComponentManager::RemovePxActorComponent(PhysicsRigidbodyComponent* component)
	{
		physics_components_.erase(component->GetPxRigidbodyActor());
	}

	Component * ComponentManager::GetComponentFromPxActor(physx::PxRigidActor * px_actor)
	{
		return physics_components_[px_actor];
	}
}
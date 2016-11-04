#pragma once

#include "pch.h"
#include "../../core/scene_graph/component.h"
#include "../../core/game_manager.h"
#include "../../core/scene_graph/component_manager.h"
#include "../../core/get.h"

namespace engine
{
	class PhysicsRigidbodyComponent : public Component
	{
	protected:
		void AddToPhysicsComponents()
		{
			Get::Manager()->component_manager_->AddPxActorComponent(this);
		}
	public:
		void Shutdown() override
		{
			Get::Manager()->component_manager_->RemovePxActorComponent(this);
		}
		virtual physx::PxRigidActor* GetPxRigidbodyActor() = 0;
	};
}
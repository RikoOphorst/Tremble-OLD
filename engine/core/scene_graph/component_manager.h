#pragma once

#include <typeindex>
#include <map>
#include <unordered_map>
#include "component_vector.h"
#include <PxPhysicsAPI.h>

namespace engine
{
	class PhysicsRigidbodyComponent;
	/**
	* @class engine::ComponentManager
	* @brief Component manager class, that holds component vectors, and runs update functions through all of them
	* 
	* @author Anton Gavrilov
	*/
	class ComponentManager
	{
	public:
		void AddPxActorComponent(PhysicsRigidbodyComponent* component);
		void RemovePxActorComponent(PhysicsRigidbodyComponent* component);
		Component* GetComponentFromPxActor(physx::PxRigidActor* px_actor);

		template<typename T>
		const std::vector<T*>& GetComponents()
		{
			std::map<std::type_index, BaseComponentVector*>::iterator iter = vectors_.find(T);
			if (iter != vectors_.end())
			{
				return ((ComponentVector<T>*)iter->second)->components_;
			}
		}

		/**
		* @brief Adds a pointer to a component to an appropriate component vector. If a vector doesn't exist - creates one
		* @param component Component that you wish to add to the updated components list
		*/
		template <class T>
		void AddComponent(T* component)
		{
			std::map<std::type_index, BaseComponentVector*>::iterator iter = vectors_.find(component->GetType());
			if (iter != vectors_.end())
			{
				((ComponentVector<T>*)(iter->second))->AddComponent(component);
			}
			else
			{
				ComponentVector<T>* new_vec = new ComponentVector<T>();
				vectors_[typeid(T)] = (BaseComponentVector*)new_vec;
				new_vec->AddComponent(component);
			}
		}

		/**
		* @brief Removes a component pointer from an appropriate vector
		* @param component Component that you want to remove
		*/
		template <class T>
		void RemoveComponent(T* component)
		{
			std::map<std::type_index, BaseComponentVector*>::iterator iter = vectors_.find(component->GetType());
			ASSERT(iter != vectors_.end() && "You are trying to delete a component, which there is 0 instances of");
			iter->second->RemoveComponent(component);
		}

		/**
		* @brief Calls void Start() on all the component vectors
		*/
		void Start()
		{
			std::map<std::type_index, BaseComponentVector*>::iterator iter;
			for (iter = vectors_.begin(); iter != vectors_.end(); iter++)
			{
				iter->second->Start();
			}
		}

		/**
		* @brief Calls void Update() on all the component vectors
		*/
		void Update()
		{
			std::map<std::type_index, BaseComponentVector*>::iterator iter;
			for (iter = vectors_.begin(); iter != vectors_.end(); iter++)
			{
				iter->second->Update();
			}
		}

		/**
		* @brief Calls void UpdateBeforePhysics() on all the component vectors
		*/
		void UpdateBeforePhysics()
		{
			std::map<std::type_index, BaseComponentVector*>::iterator iter;
			for (iter = vectors_.begin(); iter != vectors_.end(); iter++)
			{
				iter->second->UpdateBeforePhysics();
			}
		}

		/**
		* @brief Calls void UpdateAfterPhysics() on all the component vectors
		*/
		void UpdateAfterPhysics()
		{
			std::map<std::type_index, BaseComponentVector*>::iterator iter;
			for (iter = vectors_.begin(); iter != vectors_.end(); iter++)
			{
				iter->second->UpdateAfterPhysics();
			}
		}
	private:
		std::map<std::type_index, BaseComponentVector*> vectors_; //!< Map of all the component vectors, contained in this component manager, associated with their type_index
		std::unordered_map<physx::PxActor*, Component*> physics_components_;
	};
}
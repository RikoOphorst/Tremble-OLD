#pragma once
#include "../utilities/debug.h"

#include "pch.h"

namespace engine
{
	class FreeListAllocator;
	class Vector3;
	class Component;

	class PhysicsManager
	{
	public:
		friend class Allocator;
		friend class GameManager;
		friend class RigidbodyDynamic;
		friend class RigidbodyStatic;
		friend class StaticPlane;
		friend class CharacterController;


		static PhysicsManager* Create(size_t memory_size);
		static void Delete(PhysicsManager* to_delete);

		void Update(); //!< Physics manager update function. Later to be changed to be called at fixed update
		Vector3 GetGravity(); //!< Get current gravity of the world
		bool Raycast(physx::PxRaycastBuffer* buffer, Vector3 origin, Vector3 direction, float max_distance); //!< Cast a ray in the physics scene
		Component* GetComponentFromRaycast(physx::PxRaycastBuffer* buffer); //!< Get a component, that hit a ray, from the raycast buffer

		physx::PxMaterial* GetDefaultMaterial() { return px_default_material_; }

		/**
		* @brief Allocation callback class, used by physx
		*/
		class AllocatorCallback : public physx::PxAllocatorCallback
		{
		public:
			AllocatorCallback(FreeListAllocator* physx_allocator);
			virtual void* allocate(size_t size, const char* typeName, const char* filename, int line);
			virtual void deallocate(void* ptr);
		private:
			FreeListAllocator* physx_allocator_;
		};

		/**
		* @brief Error callback class, used by physx
		*/
		class ErrorCallback : public physx::PxErrorCallback
		{
		public:
			virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
		};



	private:
		PhysicsManager(FreeListAllocator* created_allocator);

		void SetTolerances(physx::PxReal length, physx::PxReal speed, physx::PxReal mass); //!< Set tolerances, used by physx. (Dependent on average measures in the world)
		bool Startup(); //!< Startup physics
		void Shutdown(); //!< Shutdown physics

		AllocatorCallback allocator_callback_;
		ErrorCallback error_callback_;

		physx::PxFoundation* px_foundation_;
		physx::PxTolerancesScale px_tolerances_scale_;
		physx::PxProfileZoneManager* px_profile_zone_manager_;
		physx::PxPhysics* px_physics_;
		physx::PxCooking* px_cooking_;
		physx::PxDefaultCpuDispatcher* px_cpu_dispatcher_;
		physx::PxScene* px_scene_;
		physx::PxVisualDebuggerConnection* px_vd_connection_;
		physx::PxControllerManager* px_controller_manager_;

		physx::PxMaterial* px_default_material_;
		physx::PxShape* px_default_box_shape_;

		FreeListAllocator* px_allocator_;
	};



}
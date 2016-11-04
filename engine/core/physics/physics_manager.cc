#include "physics_manager.h"

#include "../memory/memory_includes.h"
#include "../scene_graph/component_manager.h"
#include "../get.h"
#include "../math/math.h"

namespace engine
{
	using namespace physx;

	//------------------------------------------------------------------------------------------------------
	PhysicsManager::AllocatorCallback::AllocatorCallback(FreeListAllocator * physx_allocator)
		:physx_allocator_(physx_allocator)
	{
	
	}

	//------------------------------------------------------------------------------------------------------
	void * PhysicsManager::AllocatorCallback::allocate(size_t size, const char * typeName, const char * filename, int line)
	{
		return physx_allocator_->Allocate(size, 16);
	}

	//------------------------------------------------------------------------------------------------------
	void PhysicsManager::AllocatorCallback::deallocate(void * ptr)
	{
		if (ptr != nullptr)
		{
			return physx_allocator_->Deallocate(ptr);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void PhysicsManager::ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char * message, const char * file, int line)
	{
		std::cout << "PhysX Error: " << message << std::endl;
	}

	//------------------------------------------------------------------------------------------------------
	void PhysicsManager::Update()
	{
		px_scene_->simulate(Get::DeltaT());
		px_scene_->fetchResults(true);
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 PhysicsManager::GetGravity()
	{
		return Vector3(px_scene_->getGravity());
	}

	//------------------------------------------------------------------------------------------------------
	bool PhysicsManager::Raycast(PxRaycastBuffer* buffer, Vector3 origin, Vector3 direction, float max_distance)
	{
		px_scene_->raycast(origin.ToPxVec3(), direction.ToPxVec3(), max_distance, *buffer);
		return buffer->hasBlock;
	}

	//------------------------------------------------------------------------------------------------------
	Component* PhysicsManager::GetComponentFromRaycast(physx::PxRaycastBuffer* buffer)
	{
		PxActor* actor = buffer->block.actor;
		return Get::ComponentManager()->GetComponentFromPxActor(buffer->block.actor);
	}

	//------------------------------------------------------------------------------------------------------
	PhysicsManager* PhysicsManager::Create(size_t memory_size)
	{
		FreeListAllocator* px_allocator = Get::MemoryManager()->GetNewAllocator<FreeListAllocator>(memory_size);
		PhysicsManager* created = px_allocator->New<PhysicsManager>(px_allocator);
		if (created->Startup())
		{
			return created;
		}
		return nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	void PhysicsManager::Delete(PhysicsManager * to_delete)
	{
		to_delete->Shutdown();
		FreeListAllocator* allocator = to_delete->px_allocator_;
		allocator->Delete(to_delete);
		Get::MemoryManager()->DeleteAllocator(allocator);
	}

	//------------------------------------------------------------------------------------------------------
	PhysicsManager::PhysicsManager(FreeListAllocator * created_allocator)
		:allocator_callback_(created_allocator), px_allocator_(created_allocator)
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	void PhysicsManager::SetTolerances(PxReal length, PxReal speed, PxReal mass)
	{
		px_tolerances_scale_.length = length;
		px_tolerances_scale_.speed = speed;
		px_tolerances_scale_.mass = mass;
	}

	//------------------------------------------------------------------------------------------------------
	bool PhysicsManager::Startup()
	{
		px_foundation_ = PxCreateFoundation(PX_PHYSICS_VERSION, allocator_callback_, error_callback_);
		if (!px_foundation_)
		{
			DLOG("PhysX startup error: PxCreateFoundation failed")
			return false;
		}

		px_profile_zone_manager_ = &PxProfileZoneManager::createProfileZoneManager(px_foundation_);
		if (!px_profile_zone_manager_)
		{
			DLOG("PhysX startup error: createProfileZoneManager failed");
			return false;
		}

		bool record_memory_allocations = true;
		px_physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *px_foundation_, px_tolerances_scale_, record_memory_allocations, px_profile_zone_manager_);
		if (!px_physics_)
		{
			DLOG("PhysX startup error: PxCreatePhysics failed");
			return false;
		}

		px_cooking_ = PxCreateCooking(PX_PHYSICS_VERSION, *px_foundation_, PxCookingParams(px_tolerances_scale_));
		if (!px_cooking_)
		{
			DLOG("PhysX startup error: PxCreateCooking failed");
			return false;
		}

		if (!PxInitExtensions(*px_physics_))
		{
			DLOG("PhysX startup error: PxInitExtensions failed");
			return false;
		}


		if (px_physics_->getPvdConnectionManager())
		{
			px_physics_->getVisualDebugger()->setVisualizeConstraints(true);
			px_physics_->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
			px_physics_->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
			px_vd_connection_ = PxVisualDebuggerExt::createConnection(px_physics_->getPvdConnectionManager(), "127.0.0.1", 5425, 10);
		}
		
		px_cpu_dispatcher_ = PxDefaultCpuDispatcherCreate(4);
		PxSceneDesc px_scene_desc(px_tolerances_scale_);
		px_scene_desc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		px_scene_desc.cpuDispatcher = px_cpu_dispatcher_;
		px_scene_desc.filterShader = PxDefaultSimulationFilterShader;
		px_scene_ = px_physics_->createScene(px_scene_desc);

		px_controller_manager_ = PxCreateControllerManager(*px_scene_);

		px_default_material_ = px_physics_->createMaterial(0.5f, 0.5f, 0.6f);
		px_default_box_shape_ = px_physics_->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *px_default_material_);

		return true;
	}

	//------------------------------------------------------------------------------------------------------
	void PhysicsManager::Shutdown()
	{
		px_default_box_shape_->release();
		px_default_material_->release();

		px_controller_manager_->release();

		if (px_vd_connection_ != nullptr)
		{
			px_vd_connection_->release();
		}

		px_scene_->release();
		delete px_cpu_dispatcher_;
		PxCloseExtensions();
		px_cooking_->release();
		px_physics_->release();
		px_profile_zone_manager_->release();
		px_foundation_->release();
	}
}
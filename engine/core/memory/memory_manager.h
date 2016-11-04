#pragma once

//#include "allocators/allocator.h"
#include "allocators/free_list_allocator.h"
//#include "allocators/linear_allocator.h"
//#include "allocators/pool_allocator.h"
//#include "allocators/proxy_allocator.h"
//#include "allocators/stack_allocator.h"

#include <type_traits>

namespace engine
{
	/**
	* @brief An overall memory manager class. Handles allocation of allocators in our game.
	*
	* A single memory manager is meant to exist in a process. Different systems can allocate an allocator with a big chunk of memory for itself.
	* That allocator then manages the memory for each system.
	* @author Anton Gavrilov
	*/
	class MemoryManager
	{
	public:
		MemoryManager(size_t memory); //!< @param memory Amount of memory, that you want this memory manager to have.
		~MemoryManager(); //!< Frees all the memory in the memory manager

		/**
		* @brief Get a new allocator to manage a piece of memory for a subsystem of a game.
		* @param size Size, that you wish the allocator to have
		* @tparam AllocatorClass Class of the allocator that you want to get. Has to be a derived class from engine::Allocator
		* @return A pointer tio the initialized allocator
		*/
		template<typename AllocatorClass> 
		AllocatorClass* GetNewAllocator(size_t size)
		{
			if (std::is_base_of<Allocator, AllocatorClass>() == true)
			{
				void* p = all_allocators_->Allocate(sizeof(AllocatorClass) + size, alignof(AllocatorClass));
				return new (p) AllocatorClass(size, pointer_math::Add(p, sizeof(AllocatorClass)));
			}
			return nullptr;
		}

		/**
		* @brief Remove an allocator from the memory manager
		* @param p A pointer to the alocator that you want to deallocate
		*/
		template<typename AllocatorClass> 
		void DeleteAllocator(AllocatorClass* p)
		{
			p->~AllocatorClass();
			all_allocators_->Deallocate(p);
		}

	private:
		FreeListAllocator* all_allocators_; //!< A pointer to the free list allocator that manages all the allocators 
		void* memory_; //!< A pointer to the memory, that is allocated for the memory manager
		size_t memory_size_; //!< The size of the memory, that is in use by the memory manager
	};
}
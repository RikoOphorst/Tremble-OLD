#include "pch.h"
#include "proxy_allocator.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	ProxyAllocator::ProxyAllocator(Allocator& allocator)
		:Allocator(allocator.GetSize(), allocator.GetStart()), allocator_(allocator)
	{

	}

	//------------------------------------------------------------------------------------------------------
	ProxyAllocator::~ProxyAllocator()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void* ProxyAllocator::Allocate(size_t size, u8 alignment)
	{
		ASSERT(size != 0);
		num_allocations_++;

		size_t mem = allocator_.GetUsedMemory();

		void* p = allocator_.Allocate(size, alignment);

		used_memory_ += allocator_.GetUsedMemory() - mem;

		return p;
	}

	//------------------------------------------------------------------------------------------------------
	void ProxyAllocator::Deallocate(void* p)
	{
		num_allocations_--;

		size_t mem = allocator_.GetUsedMemory();

		allocator_.Deallocate(p);

		used_memory_ -= mem - allocator_.GetUsedMemory();
	}
}
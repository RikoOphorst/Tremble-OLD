#include "pch.h"
#include "linear_allocator.h"

namespace engine
{

	LinearAllocator::LinearAllocator(size_t size, void* start)
		:Allocator(size, start), current_pos_(start)
	{
		ASSERT(size > 0);
	}

	//------------------------------------------------------------------------------------------------------
	LinearAllocator::~LinearAllocator()
	{
		current_pos_ = nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	void* LinearAllocator::Allocate(size_t size, u8 alignment)
	{
		ASSERT(size != 0);

		u8 adjustment = pointer_math::AlignForwardAdjustment(current_pos_, alignment);

		if (used_memory_ + adjustment + size > size_)
		{
			DLOG("New allocation inside a linear allocator could not complete. Not enough memory in the allocator")
			return nullptr;
		}

		uptr aligned_address = (uptr)current_pos_ + adjustment;

		current_pos_ = (void*)(aligned_address + size);

		used_memory_ += size + adjustment;
		num_allocations_++;


		return (void*)aligned_address;
	}

	//------------------------------------------------------------------------------------------------------
	void LinearAllocator::Deallocate(void *p)
	{
		ASSERT(false && "Cannot deallocate one object with a linear allocator. If you wish to clear the whole allocator, use clear() instead");
	}

	//------------------------------------------------------------------------------------------------------
	void LinearAllocator::clear()
	{
		num_allocations_ = 0;
		used_memory_ = 0;
		current_pos_ = start_;
	}
}
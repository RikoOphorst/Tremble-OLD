#pragma once

#include "gpu_buffer.h"

namespace engine
{
	class ByteAddressBuffer : public GpuBuffer
	{
	public:
		virtual void CreateDerivedViews() override;
	};
}
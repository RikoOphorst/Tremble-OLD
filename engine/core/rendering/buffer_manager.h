#pragma once

#include "color_buffer.h"
#include "depth_buffer.h"
#include "byte_address_buffer.h"
#include "typed_buffer.h"
#include "structured_buffer.h"

#include <unordered_map>

#include "../get.h"

namespace engine
{
	class BufferManager
	{
	public:
		BufferManager();
		~BufferManager();

		void Create(UINT width, UINT height);
		void Destroy();

		inline static ColorBuffer& GetColorBuffer(const std::string& name);
		inline static DepthBuffer& GetDepthBuffer(const std::string& name);
		inline static ByteAddressBuffer& GetByteAddressBuffer(const std::string& name);
		/*inline static TypedBuffer& GetTypedBuffer(const std::string& name);*/
		inline static StructuredBuffer& GetStructuredBuffer(const std::string& name);

	private:
		std::unordered_map<std::string, ColorBuffer> buffers_color_;
		std::unordered_map<std::string, DepthBuffer> buffers_depth_;
		std::unordered_map<std::string, ByteAddressBuffer> buffers_byte_address_;
		/*std::unordered_map<std::string, TypedBuffer> buffers_typed_;*/
		std::unordered_map<std::string, StructuredBuffer> buffers_structured_;
	};

	inline ColorBuffer&			BufferManager::GetColorBuffer(const std::string& name)			{ return Get::BufferManager().buffers_color_[name]; }
	inline DepthBuffer&			BufferManager::GetDepthBuffer(const std::string& name)			{ return Get::BufferManager().buffers_depth_[name]; }
	inline ByteAddressBuffer&	BufferManager::GetByteAddressBuffer(const std::string& name)	{ return Get::BufferManager().buffers_byte_address_[name]; }
	/*inline TypedBuffer&			BufferManager::GetTypedBuffer(const std::string& name)			{ return Get::BufferManager().buffers_typed_[name]; }*/
	inline StructuredBuffer&	BufferManager::GetStructuredBuffer(const std::string& name)		{ return Get::BufferManager().buffers_structured_[name]; }

	inline ColorBuffer&			GetColorBuffer(const std::string& name)			{ return BufferManager::GetColorBuffer(name); }
	inline DepthBuffer&			GetDepthBuffer(const std::string& name)			{ return BufferManager::GetDepthBuffer(name); }
	inline ByteAddressBuffer&	GetByteAddressBuffer(const std::string& name)	{ return BufferManager::GetByteAddressBuffer(name); }
	/*inline TypedBuffer&			GetTypedBuffer(const std::string& name)			{ return BufferManager::GetTypedBuffer(name); }*/
	inline StructuredBuffer&	GetStructuredBuffer(const std::string& name)	{ return BufferManager::GetStructuredBuffer(name);	}
}
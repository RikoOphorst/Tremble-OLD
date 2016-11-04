#pragma once

#include "direct3d.h"
#include "device.h"
#include "renderer.h"
#include "../get.h"

namespace engine
{
	template<typename T>
	class UploadBuffer
	{
	public:
		UploadBuffer(UINT element_count, bool is_constant_buffer = false)
		{
			element_byte_size_ = sizeof(T);
			if (is_constant_buffer)
			{
				// constant buffers need to have byte size of >=256
				element_byte_size_ = (sizeof(T) + 255) & ~255;
			}

			CHECKHR(
				Get::Renderer()->GetDevice().Get()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(element_byte_size_ * element_count),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&upload_buffer_)
				)
			);

			CHECKHR(upload_buffer_->Map(0, nullptr, reinterpret_cast<void**>(&mapped_data)));
		}

		UploadBuffer(const UploadBuffer& rhs) = delete;
		UploadBuffer& operator=(const UploadBuffer& rhs) = delete;

		~UploadBuffer()
		{
			upload_buffer_->Unmap(0, nullptr);
			GUARANTEE_RELEASE(upload_buffer_);
		}

		ID3D12Resource* GetBuffer() const { return upload_buffer_; }
		UINT GetElementByteSize() const { return element_byte_size_; }

		void CopyData(int element_index, const T& data)
		{
			memcpy(&mapped_data[element_index * element_byte_size_], &data, sizeof(T));
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetAddressById(UINT id) 
		{
			auto address = upload_buffer_->GetGPUVirtualAddress();
			address += element_byte_size_ * id;

			return address;
		}

	private:
		ID3D12Resource* upload_buffer_;
		BYTE* mapped_data = nullptr;

		UINT element_byte_size_;
		bool is_constant_buffer_;
	};
}
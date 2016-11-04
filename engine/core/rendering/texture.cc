#include "texture.h"

#include "../win32/wic_loader.h"
#include "../game_manager.h"
#include "renderer.h"
#include "device.h"
#include "descriptor_heap.h"
#include "../get.h"
#include "command_context.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	Texture::Texture(const std::string& file_path) :
		file_path_(file_path),
		buffers_built_(false)
	{
		LoadFromFile(file_path_);
	}

	//------------------------------------------------------------------------------------------------------
	Texture::~Texture()
	{
		SAFE_DELETE(loader_);
	}

	//------------------------------------------------------------------------------------------------------
	void Texture::LoadFromFile(const std::string& file_path)
	{
		loader_ = new WICLoader();

		texture_data_ = loader_->LoadFromFile(file_path);
		buffers_built_ = false;
	}

	//------------------------------------------------------------------------------------------------------
	void Texture::BuildBuffers()
	{
		if (texture_data_.success != true)
		{
			return;
		}

		D3D12_RESOURCE_DESC desc;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = texture_data_.image_width;
		desc.Height = texture_data_.image_height;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = texture_data_.dxgi_format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		CHECKHR(
			Get::Device().Get()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&resource_)
			)
		);
		resource_->SetName(L"Texture");

		D3D12_SUBRESOURCE_DATA resource_data = {};
		resource_data.pData = &texture_data_.image_data[0];
		resource_data.RowPitch = texture_data_.bytes_per_row;
		resource_data.SlicePitch = texture_data_.bytes_per_row * texture_data_.image_height;
		CommandContext::InitializeTexture(*this, 1, &resource_data);

		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv_desc.Format = texture_data_.dxgi_format;
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = desc.MipLevels;
		srv_desc.Texture2D.MostDetailedMip = 0;
		srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;
		srv_desc.Texture2D.PlaneSlice = 0;

		srv_id_ = Get::CbvSrvUavHeap().CreateShaderResourceView(resource_, &srv_desc);

		buffers_built_ = true;
	}
}
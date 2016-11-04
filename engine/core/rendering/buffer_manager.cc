#include "pch.h"
#include "buffer_manager.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	BufferManager::BufferManager()
	{

	}

	//------------------------------------------------------------------------------------------------------
	BufferManager::~BufferManager()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void BufferManager::Create(UINT width, UINT height)
	{
		buffers_color_["scene_color"].Create(L"BufferSceneColor", width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
		buffers_depth_["scene_depth"].Create(L"BufferSceneDepth", width, height, DXGI_FORMAT_D32_FLOAT);
	}

	//------------------------------------------------------------------------------------------------------
	void BufferManager::Destroy()
	{
		buffers_color_.clear();
		buffers_depth_.clear();
		buffers_byte_address_.clear();
		/*buffers_typed_.clear();*/
		buffers_structured_.clear();
	}
}
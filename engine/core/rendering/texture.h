#pragma once

#include "../../pch.h"
#include "../win32/wic_loader.h"
#include "gpu_resource.h"

namespace engine
{
	class GameManager;

	class Texture : public GpuResource
	{
	public:
		Texture(const std::string& file_path);
		~Texture();

		void LoadFromFile(const std::string& file_path);
		void BuildBuffers();
		bool AreBuffersBuilt() const { return buffers_built_; }

		const WICLoadedData& GetTextureData() const { return texture_data_; };
		const UINT& GetSRV() const { return srv_id_; }
	private:
		std::string file_path_;

		WICLoader* loader_;
		WICLoadedData texture_data_;
		UINT srv_id_;
		bool buffers_built_;
	};
}
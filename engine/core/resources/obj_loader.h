#pragma once

#include "mesh.h"

namespace engine
{
	class OBJLoader
	{
	public:
 		static bool LoadOBJ(const std::string& file_path_to_obj, Mesh::MeshData* mesh_data);
	};
}
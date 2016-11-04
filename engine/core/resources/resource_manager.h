#pragma once

#include "pch.h"
#include "mesh.h"
#include "../rendering/shader.h"

namespace engine
{
	class GameManager;
	class FreeListAllocator;
	struct Material;

	class ResourceManager
	{
	public:
		/**
		* @brief Resource Manager default constructor
		* @param game_manager Pointer to a game manager that you want this resource manager to be connected to
		* @param memory_size Size of memory, that you want this resource manager to haves
		*/
		ResourceManager(GameManager* game_manager, size_t memory_size);
		~ResourceManager(); //!< Destructor. Frees all resources and deletes allocator

		Mesh* GetMesh(FbxMesh* fbx_mesh, const std::string& file_path);
		Mesh* GetMesh(const std::string& file_path);

		Material* GetMaterial(const std::string& name) { return nullptr; }
		Material* GetMaterial(FbxSurfaceMaterial* fbx_material, const std::string& file_name);

		Texture* GetTexture(const std::string& file_path);

		Shader* GetShader(const std::string& file_path, const Shader::ShaderType& shader_type);
	private:
		GameManager* game_manager_; //!< Pointer to a game manager, that this resource manager uses
		FreeListAllocator* resource_allocator_; //!< allocator that resource manager uses internally

		std::unordered_map<std::string, Texture*> textures_; //!< Unordered map of texture pointers, associated with names(file_paths). Resource manager owns these textures
		std::unordered_map<std::string, Mesh*> meshes_; //!< Unordered map of mesh pointers, associated with names(file_paths). Resource manager owns these meshes
		std::unordered_map<std::string, Material*> materials_; //!< Unordered map of material pointers, associated with names(file_paths). Resource manager owns these materials
		std::unordered_map<std::string, Shader*> shaders_; //!< Unordered map of material pointers, associated with names(file_paths). Resource manager owns these materials
	};
}
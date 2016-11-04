#include "pch.h"
#include "resource_manager.h"

#include "mesh.h"
#include "obj_loader.h"
#include "fbx_loader.h"
#include "../rendering/texture.h"
#include "../rendering/material.h"
#include "../memory/memory_includes.h"
#include "../game_manager.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	ResourceManager::ResourceManager(GameManager* game_manager, size_t memory_size)
		:game_manager_(game_manager)
	{
		resource_allocator_ = game_manager_->GetMemoryManager()->GetNewAllocator<FreeListAllocator>(memory_size);
	}

	//------------------------------------------------------------------------------------------------------
	ResourceManager::~ResourceManager()
	{
		for each (auto pair in meshes_)
		{
			resource_allocator_->Delete(pair.second);
		}

		for each (auto pair in textures_)
		{
			resource_allocator_->Delete(pair.second);
		}

		for each (auto pair in materials_)
		{
			resource_allocator_->Delete(pair.second);
		}

		for each (auto pair in shaders_)
		{
			resource_allocator_->Delete(pair.second);
		}

		game_manager_->GetMemoryManager()->DeleteAllocator(resource_allocator_);
	}

	//------------------------------------------------------------------------------------------------------
	Mesh* ResourceManager::GetMesh(const std::string& file_path)
	{
		std::unordered_map<std::string, Mesh*>::iterator iter = meshes_.find(file_path);
		//If id found in the container
		if (iter != meshes_.end())
		{
			//return id
			return iter->second;
		}
		else //if name not already existent
		{
			if (file_path.find(".obj") != std::string::npos || file_path.find(".OBJ") != std::string::npos)
			{
				Mesh::MeshData* data = resource_allocator_->New<Mesh::MeshData>();
				OBJLoader::LoadOBJ(file_path, data);
				Mesh* mesh = resource_allocator_->New<Mesh>(*data);
				meshes_[file_path] = mesh;
				return mesh;
			}
			else if (file_path.find(".fbx") != std::string::npos || file_path.find(".FBX") != std::string::npos)
			{
				Mesh::MeshData* data = resource_allocator_->New<Mesh::MeshData>();
				FBXLoader::LoadFBXMesh(file_path, data);
				Mesh* mesh = resource_allocator_->New<Mesh>(*data);
				meshes_[file_path] = mesh;
				return mesh;
			}
			else
			{
				DLOG("Trying to load an unsupported file format");
				return 0;
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	Texture* ResourceManager::GetTexture(const std::string& file_path)
	{
		std::string name = file_path;
		std::unordered_map<std::string, Texture*>::iterator iter = textures_.find(name);
		if (iter != textures_.end())
		{
			//return texture ptr
			return iter->second;
		}
		else
		{
			Texture* texture = resource_allocator_->New<Texture>(file_path);
			textures_[name] = texture;
			return texture;
		}
	}

	//------------------------------------------------------------------------------------------------------
	Shader* ResourceManager::GetShader(const std::string& file_path, const Shader::ShaderType& shader_type)
	{
		std::string name = file_path + std::to_string(static_cast<int>(shader_type));
		auto iter = shaders_.find(name);

		if (iter != shaders_.end())
		{
			return iter->second;
		}
		else
		{
			Shader* shader = resource_allocator_->New<Shader>();
			shader->CompileFromFile(file_path, shader_type);

			shaders_[name] = shader;
			return shader;
		}
	}

	//------------------------------------------------------------------------------------------------------
	Material* ResourceManager::GetMaterial(FbxSurfaceMaterial* fbx_material, const std::string& file_path)
	{
		std::string name = file_path + std::to_string(fbx_material->GetUniqueID());
		std::unordered_map<std::string, Material*>::iterator iter = materials_.find(name);
		if (iter != materials_.end())
		{
			//return texture ptr
			return iter->second;
		}
		else
		{
			Material* material = resource_allocator_->New<Material>();
			FBXLoader::ImportFBXMaterial(fbx_material, material, this, file_path);
			materials_[name] = material;
			return material;
		}
	}

	//------------------------------------------------------------------------------------------------------
	Mesh* ResourceManager::GetMesh(FbxMesh* fbx_mesh, const std::string& file_path)
	{
		std::string name = file_path + std::to_string(fbx_mesh->GetUniqueID());
		std::unordered_map<std::string, Mesh*>::iterator iter = meshes_.find(name);
		if (iter != meshes_.end())
		{
			//return mesh ptr
			return iter->second;
		}
		else
		{
			Mesh::MeshData data;
			FBXLoader::ImportFBXMesh(fbx_mesh, &data);
			Mesh* mesh = resource_allocator_->New<Mesh>(data);
			meshes_[name] = mesh;
			return mesh;
		}
	}
}
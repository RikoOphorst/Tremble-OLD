#include "pch.h"
//#include "assimp_loader.h"
//
//#include <vector>
//
//namespace engine
//{
//	AssimpLoader* AssimpLoader::instance_ = nullptr;
//
//	//------------------------------------------------------------------------------------------------------
//	AssimpLoader::AssimpLoader(GameManager* game_manager) :
//		game_manager_(game_manager)
//	{
//		if (instance_ != nullptr)
//		{
//			delete instance_;
//		}
//
//		instance_ = this;
//	}
//	
//	//------------------------------------------------------------------------------------------------------
//	AssimpLoader::~AssimpLoader()
//	{
//
//	}
//	
//	//------------------------------------------------------------------------------------------------------
//	AssimpLoader* AssimpLoader::Instance()
//	{
//		return nullptr;
//	}
//	
//	//------------------------------------------------------------------------------------------------------
//	void AssimpLoader::LoadScene(const std::string& file_path, SGNode* node)
//	{
//		Assimp::Importer importer;
//		const aiScene* scene = importer.ReadFile(file_path,
//			aiProcess_CalcTangentSpace |
//			aiProcess_Triangulate |
//			aiProcess_JoinIdenticalVertices |
//			aiProcess_SortByPType);
//
//		assert(scene);
//
//		assert(scene->HasMeshes());
//
//		std::vector<Mesh*> meshes_;
//		std::vector<Material*> materials_;
//
//		meshes_.resize(scene->mNumMeshes);
//		materials_.resize(scene->mNumMaterials);
//
//
//
//		// materials
//		for (int i = 0; i < static_cast<int>(scene->mNumMaterials); i++)
//		{
//			Material* mat = new Material();
//			aiMaterial* material = scene->mMaterials[i];
//
//			GET_COLOR(mat->diffuse, mat->diffuse_map, mat->use_diffuse_map, game_manager_, aiTextureType_DIFFUSE, AI_MATKEY_COLOR_DIFFUSE);
//			GET_COLOR(mat->emissive, mat->emissive_map, mat->use_emissive_map, game_manager_, aiTextureType_EMISSIVE, AI_MATKEY_COLOR_EMISSIVE);
//			GET_COLOR(mat->ambient_reflectance, mat->ambient_map, mat->use_ambient_map, game_manager_, aiTextureType_AMBIENT, AI_MATKEY_COLOR_AMBIENT);
//			GET_COLOR(mat->specular, mat->specular_map, mat->use_specular_map, game_manager_, aiTextureType_SPECULAR, AI_MATKEY_COLOR_DIFFUSE);
//
//			float shininess;
//			material->Get(AI_MATKEY_SHININESS, shininess);
//			mat->shininess = shininess;
//
//			materials_.push_back(mat);
//		}
//
//
//
//		// meshes 
//		aiNode* root_node = scene->mRootNode;
//
//		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
//		const aiColor4D ZeroColor(0.0f, 0.0f, 0.0f, 1.0f);
//
//		for (int i = 0; i < static_cast<int>(root_node->mNumMeshes); i++)
//		{
//			RenderableMesh* r_mesh = node->AddComponent<RenderableMesh>();
//			Mesh::MeshData mesh_data;
//
//			mesh_data.topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//
//			aiMesh* mesh = scene->mMeshes[root_node->mMeshes[i]];
//
//			for (int j = 0; j < static_cast<int>(mesh->mNumVertices); j++)
//			{
//				Vertex vert;
//
//				const aiVector3D* position = &(mesh->mVertices[j]);
//				const aiVector3D* normal = mesh->HasNormals() ? &(mesh->mNormals[j]) : &Zero3D;
//				const aiVector3D* bitangent = mesh->HasTangentsAndBitangents() ? &(mesh->mBitangents[j]) : &Zero3D;
//				const aiVector3D* tangent = mesh->HasTangentsAndBitangents() ? &(mesh->mTangents[j]) : &Zero3D;
//				const aiVector3D* tex_coord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][j]) : &Zero3D;
//				const aiColor4D* color = mesh->HasVertexColors(0) ? mesh->mColors[0] : &ZeroColor;
//
//				vert.position = DirectX::XMFLOAT3(position->x, position->y, position->z);
//				vert.color = DirectX::XMFLOAT4(color->r, color->g, color->b, color->a);
//				vert.normal = DirectX::XMFLOAT3(normal->x, normal->y, normal->z);
//				vert.bitangent = DirectX::XMFLOAT3(bitangent->x, bitangent->y, bitangent->z);
//				vert.tangent = DirectX::XMFLOAT3(tangent->x, tangent->y, tangent->z);
//				vert.uv = DirectX::XMFLOAT2(tex_coord->x, tex_coord->y);
//
//				mesh_data.vertices.push_back(vert);
//			}
//
//			for (int j = 0; j < static_cast<int>(mesh->mNumFaces); j++)
//			{
//				aiFace face = mesh->mFaces[j];
//
//				assert(face.mNumIndices == 3);
//
//				for (int k = 0; k < static_cast<int>(face.mNumIndices); k++)
//				{
//					mesh_data.indices.push_back(face.mIndices[k]);
//				}
//			}
//			
//			Mesh* tmesh = new Mesh(mesh_data);
//			r_mesh->SetMesh(tmesh);
//			r_mesh->SetMaterial(materials_[mesh->mMaterialIndex]);
//		}
//	}
//}
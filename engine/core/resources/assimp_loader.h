//#pragma once
//
//#include "../sg_entity_component/scene_graph.h"
//#include "../rendering/material.h"
//#include "../resource_management/mesh.h"
//
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing fla
//
//#include <string>
//
//#define GET_COLOR(key, map, use_map, game_man, tx_type, mat_key) \
//if (material->GetTextureCount(tx_type) > 0) \
//{\
//	aiString path;\
//	aiReturn returned = material->GetTexture(tx_type, 0, &path);\
//	assert(returned == aiReturn_SUCCESS);\
//\
//	map = new Texture(path.C_Str());\
//	use_map = true;\
//}\
//else\
//{\
//	aiColor4D color;\
//	assert(material->Get(mat_key, color) == aiReturn_SUCCESS);\
//	key = DirectX::XMFLOAT3(color.r, color.g, color.b);\
//}
//
//namespace engine
//{
//	class AssimpLoader
//	{
//	public:
//		AssimpLoader(GameManager* game_manager);
//		~AssimpLoader();
//
//		static AssimpLoader* Instance();
//
//		void LoadScene(const std::string& file_path, SGNode* node);
//		void LoadMesh(aiScene* scene, aiMesh* mesh);
//
//	private:
//		GameManager* game_manager_;
//		static AssimpLoader* instance_;
//	};
//}
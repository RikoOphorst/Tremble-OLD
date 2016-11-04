#pragma once

#include "../../pch.h"
#include "../rendering/vertex.h"
#include "../rendering/structured_buffer.h"
#include "../rendering/byte_address_buffer.h"
#include "../rendering/graphics_context.h"

namespace engine
{
	class Texture;
	/**
	* @class engine::Mesh
	* @author Riko Ophorst
	* @brief Allows to create meshes given model data
	*/
	class Mesh
	{
	public:
		/**
		* @struct engine::Mesh::MeshData
		* @author Riko Ophorst
		* @brief Describes & stores the data of a mesh
		*/
		struct MeshData
		{
			std::vector<Vertex> vertices; //!< All vertices in this mesh
			std::vector<uint16_t> indices; //!< All indices in this mesh
			D3D_PRIMITIVE_TOPOLOGY topology; //!< Topology of this mesh
		};

		/**
		* @brief Constructs a mesh based on supplied mesh data
		* @param[in] mesh_data The mesh data the mesh should be build with
		*/
		Mesh(const MeshData& mesh_data);

		/**
		* @brief Mesh destructor
		*/
		~Mesh();

		/**
		* @brief Loads in a mesh based on filepath
		* @param[in] file_path_to_obj The path to the obj you want to load
		*/

		void SetMeshData(const MeshData& mesh_data) { mesh_data_ = mesh_data; }
		const MeshData& GetMeshData() { return mesh_data_; }

		void SetVertices(const std::vector<Vertex>& vertices);
		std::vector<Vertex> GetVertices() const { return mesh_data_.vertices; }

		void SetIndices(const std::vector<uint16_t>& indices);
		std::vector<uint16_t> GetIndices() const { return mesh_data_.indices; }

		size_t GetIndexCount() const { return mesh_data_.indices.size(); }

		void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology) { topology_ = topology; }
		D3D12_PRIMITIVE_TOPOLOGY GetTopology() const { return topology_; }

		void BuildBuffers();

		void Set(GraphicsContext& context);
		void Draw(GraphicsContext& context);

		bool AreBuffersBuilt() const { return buffers_built_; }

		const DirectX::BoundingSphere& GetBounds() const { return bounds_; }

	protected:
		MeshData mesh_data_;

		D3D12_PRIMITIVE_TOPOLOGY topology_;

		StructuredBuffer vertex_buffer_;
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;

		ByteAddressBuffer index_buffer_;
		D3D12_INDEX_BUFFER_VIEW index_buffer_view_;

		bool buffers_built_;

		DirectX::BoundingSphere bounds_;
	};
}

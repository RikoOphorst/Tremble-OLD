#include "mesh.h"

#include <map>
#include <sstream>
#include <iostream>
#include <utility>

#include "../rendering/renderer.h"
#include "../utilities/utilities.h"
#include "../game_manager.h"
#include "../get.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	Mesh::Mesh(const MeshData& mesh_data) :
		topology_(mesh_data.topology),
		buffers_built_(false)
	{
		mesh_data_ = std::move(mesh_data);
	}

	//------------------------------------------------------------------------------------------------------
	Mesh::~Mesh()
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	void Mesh::SetVertices(const std::vector<Vertex>& vertices)
	{
		mesh_data_.vertices = std::move(vertices);
	}

	//------------------------------------------------------------------------------------------------------
	void Mesh::SetIndices(const std::vector<uint16_t>& indices)
	{
		mesh_data_.indices = std::move(indices);
	}

	//------------------------------------------------------------------------------------------------------
	void Mesh::BuildBuffers()
	{
		vertex_buffer_.Create(L"VertexBuffer", static_cast<UINT>(mesh_data_.vertices.size()), sizeof(Vertex), &mesh_data_.vertices[0]);

		vertex_buffer_view_ = {};
		vertex_buffer_view_.StrideInBytes = sizeof(Vertex);
		vertex_buffer_view_.SizeInBytes = static_cast<UINT>(mesh_data_.vertices.size() * sizeof(Vertex));
		vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();

		if (mesh_data_.indices.size() > 0)
		{
			index_buffer_.Create(L"IndexBuffer", static_cast<UINT>(mesh_data_.indices.size()), static_cast<UINT>(sizeof(uint16_t)), &mesh_data_.indices[0]);

			index_buffer_view_ = {};
			index_buffer_view_.Format = DXGI_FORMAT_R16_UINT;
			index_buffer_view_.SizeInBytes = static_cast<UINT>(mesh_data_.indices.size() * sizeof(uint16_t));
			index_buffer_view_.BufferLocation = index_buffer_->GetGPUVirtualAddress();
		}

		DirectX::BoundingSphere::CreateFromPoints(bounds_, mesh_data_.vertices.size(), &mesh_data_.vertices[0].position, sizeof(Vertex));

		buffers_built_ = true;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Mesh::Set(GraphicsContext& context)
	{
		if (!AreBuffersBuilt())
		{
			BuildBuffers();
		}

		context.SetPrimitiveTopology(topology_);
		context.SetVertexBuffer(0, vertex_buffer_view_);
		if (mesh_data_.indices.size() > 0)
		{
			context.SetIndexBuffer(index_buffer_view_);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Mesh::Draw(GraphicsContext& context)
	{
		if (!AreBuffersBuilt())
		{
			BuildBuffers();
		}

		context.SetPrimitiveTopology(topology_);
		context.SetVertexBuffer(0, vertex_buffer_view_);

		if (mesh_data_.indices.size() > 0)
		{
			context.SetIndexBuffer(index_buffer_view_);
			context.DrawIndexedInstanced(static_cast<UINT>(mesh_data_.indices.size()), 1, 0, 0, 0);
		}
		else
		{
			context.DrawInstanced(static_cast<UINT>(mesh_data_.vertices.size()), 1);
		}
	}
}
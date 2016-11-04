#include "pch.h"
#include "renderable.h"

#include "../../core/resources/mesh.h"
#include "../../core/rendering/material.h"
#include "../../core/rendering/texture.h"
#include "../../core/scene_graph/scene_graph.h"

#define CHECK_BUILD_BUFFERS(texture) if (texture != nullptr && !texture->AreBuffersBuilt()) { texture->BuildBuffers(); }

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	Renderable::Renderable() :
		mesh_(nullptr),
		material_(nullptr),
		renderer_object_cb_id_(0),
		renderer_object_descriptor_id_(0)
	{
		
	}
	
	//------------------------------------------------------------------------------------------------------
	void Renderable::Start()
	{
		CHECK_BUILD_BUFFERS(mesh_);
		CHECK_BUILD_BUFFERS(material_->emissive_map);
		CHECK_BUILD_BUFFERS(material_->ambient_map);
		CHECK_BUILD_BUFFERS(material_->diffuse_map);
		CHECK_BUILD_BUFFERS(material_->specular_map);
		CHECK_BUILD_BUFFERS(material_->shininess_map);
		CHECK_BUILD_BUFFERS(material_->normal_map);

		ComputeBounds();
	}
	
	//------------------------------------------------------------------------------------------------------
	void Renderable::Update()
	{
		GetNode()->GetWorldPosition();
	}
	
	//------------------------------------------------------------------------------------------------------
	void Renderable::ComputeBounds()
	{
		Mesh::MeshData mesh = mesh_->GetMeshData();

		for (int i = 0; i < mesh.vertices.size(); i++)
		{
			mesh.vertices[i].position = Mat44(GetNode()->GetWorldTransform()) * Vector4(mesh.vertices[i].position, Scalar(1.0f));
		}

		DirectX::BoundingBox::CreateFromPoints(bounding_box_, mesh.vertices.size(), &mesh.vertices[0].position, sizeof(Vertex));
	}

	//------------------------------------------------------------------------------------------------------
	const DirectX::BoundingBox& Renderable::GetBoundingBox()
	{
		return bounding_box_;
	}
}
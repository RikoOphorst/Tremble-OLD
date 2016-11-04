#pragma once

#include "../../core/rendering/direct3d.h"
#include "../../core/scene_graph/component.h"
#include <iostream>

namespace engine
{
	class Mesh;
	class Texture;
	struct Material;

	class Renderable : public Component
	{
		friend class FBXLoader;
	public:
		Renderable();

		void Start();
		void Update();

		void SetMesh(Mesh* mesh) { mesh_ = mesh; }
		Mesh* GetMesh() { return mesh_; }

		void SetMaterial(Material* material) { material_ = material; }
		Material* GetMaterial() { return material_; }

		void SetRendererObjectCBId(UINT id) { renderer_object_cb_id_ = id; }
		UINT GetRendererObjectCBId() const { return renderer_object_cb_id_; }

		void SetRendererObjectDescriptorId(UINT id) { renderer_object_descriptor_id_ = id; }
		UINT GetRendererObjectDescriptorId() const { return renderer_object_descriptor_id_; }

		void ComputeBounds();
		const DirectX::BoundingBox& GetBoundingBox();
	private:
		Mesh* mesh_;
		Material* material_;
		UINT renderer_object_cb_id_;
		UINT renderer_object_descriptor_id_;
		DirectX::BoundingBox bounding_box_;
	};
}
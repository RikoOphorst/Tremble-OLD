#pragma once

#include "pch.h"
#include "debug_volume.h"
#include "constant_buffers.h"

#include "device.h"
#include "swap_chain.h"
#include "depth_buffer.h"
#include "descriptor_heap.h"
#include "buffer_manager.h"

namespace engine
{
	class Window;
	class RootSignature;
	class Shader;
	class Camera;
	class Renderable;
	class Light;
	struct Material;
	class Timer;
	class GameManager;
	class DepthStencil;
	class Octree;
	class CommandContext;
	class GraphicsContext;

	template<typename T>
	class UploadBuffer;
	
	/**
	* @class engine::Renderer
	* @author Riko Ophorst
	* @brief Forward DirectX 12 renderer
	*/
	class Renderer
	{
	public:
		/**
		* @brief Renderer constructor
		*/
		Renderer();

		/**
		* @brief Renderer destructor
		*/
		~Renderer();

		/**
		* @brief Starts up the renderer, initializes everything, etc.
		*/
		void Startup();

		/**
		* @brief Makes the renderer draw to screen
		* @param[in] gt The timer the game is running with
		*/
		void Draw(Timer* gt);

		void SetCamera(Camera* camera);

		void BuildOctree();

		Device& GetDevice(); //!< Get the underlying device
		const Device& GetDevice() const; //!< Get the underlying device
		
		DescriptorHeap& GetRtvHeap() { return rtv_heap_; }
		DescriptorHeap& GetDsvHeap() { return dsv_heap_; 
		}
		DescriptorHeap& GetCbvSrvUavHeap() { return cbv_srv_uav_heap_; }
		DescriptorHeap& GetSamplerHeap() { return sampler_heap_; }

		SwapChain& GetSwapChain() { return swap_chain_; }

		inline BufferManager& GetBufferManager() { return buffer_manager_; }

	public:
		void AddRenderable(Renderable* renderable);
		void DestroyRenderable(Renderable* renderable);
		void DestroyRenderableById(int id);
		Renderable* GetRenderableById(int id);
		int GetIdOfRenderable(Renderable* renderable);

		void RenderDebugVolume(const DebugVolume& volume);

		void AddLight(Light* light);
		void DestroyLight(Light* light);

	private:
		void CreateDevice(); //!< Creates the Direct3D device
		void CreateCommandObjects(); //!< Creates the command objects (queue, allocator, list)
		void CreateSwapChain(); //!< Creates the swap chain
		void CreateDescriptorHeaps(); //!< Creates the resource heaps for the render target views & depth/stencil views
		void CreateDepthStencilView(); //!< Creates the depth/stencil view
		void CreateRootSignature(); //!< Creates the root signature
		void CreateDefaultPSO(); //!< Creates the default pipeline state object
		void CreateFrameResources(); //!< Creates all the buffered frame resources
		void CreateSamplerDescs(); //!< Creates all the static sampler descs that are available in shaders to sample textures
		void FlushCommandQueue(); //!< Informs the renderer to flush the entire command queue

		void ComputeCulling(std::vector<Renderable*>& out_renderables);
		void DrawRenderables(GraphicsContext& context, std::vector<Renderable*>& renderables);
		void DrawDebugVolumes(GraphicsContext& context, std::vector<DebugVolume>& debug_volumes);

		void ProcessPendingRenderables();
	private:
		Window* output_window_;

		Device device_;
		SwapChain swap_chain_;
		DepthBuffer depth_buffer_;

		HWND main_window_handle_; //!< The main window used to render to

		DescriptorHeap rtv_heap_; //!< Heap to store descriptors of the render target views
		DescriptorHeap dsv_heap_; //!< Heap to store descriptors of the depth/stencil views
		DescriptorHeap cbv_srv_uav_heap_; //!< Heap to store descriptors of shader resource views
		DescriptorHeap sampler_heap_; //!< Heap to store descriptors of sampler states

		UploadBuffer<PassConstants>* pass_constants_;
		UploadBuffer<MaterialConstants>* material_constants_;
		UploadBuffer<ObjectConstants>* object_constants_;
		UploadBuffer<DebugConstants>* debug_constants_;

		UINT num_object_cbs_;
		UINT num_material_cbs_;

		UINT pass_cb_descriptor_id_;

		std::vector<CD3DX12_STATIC_SAMPLER_DESC> sampler_descs_; //!< Contains all the static samplers shaders can use to sample textures

		Shader* vertex_shader_; //!< Vertex shader that's used as the default vertex shader
		Shader* pixel_shader_; //!< Pixel shader that's used as the default pixel shader
		Shader* debug_vertex_shader_; //!< Vertex shader that's used as the default vertex shader
		Shader* debug_pixel_shader_; //!< Pixel shader that's used as the default pixel shader

		std::vector<Light*> lights_;
		std::vector<Material*> materials_;

		Octree* octree_;
		bool is_rendering_;

		BufferManager buffer_manager_;
	private:
		std::queue<Renderable*> renderables_to_add_; //!< All the renderables that should be added to the renderables list
		std::queue<Renderable*> renderables_to_remove_; //!< All the renderables that should be removed from the renderables list
		std::vector<Renderable*> renderables_; //!< The list of renderables that will be rendered each frame
		std::vector<DebugVolume> debug_volumes_; //!< The queue of debug volumes that will be rendered each frame

		Camera* camera_; //!< The camera that is used to render the scene with
	};
}
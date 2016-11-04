#include "renderer.h"
#include "renderer.h"
#include "renderer.h"
#include "renderer.h"
#include "renderer.h"
#include "renderer.h"

#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>

#include "command_context_manager.h"
#include "command_manager.h"
#include "command_context.h"
#include "graphics_context.h"
#include "command_queue.h"
#include "../utilities/debug.h"
#include "../win32/window.h"
#include "../resources/mesh.h"
#include "../resources/resource_manager.h"
#include "../utilities/timer.h"
#include "../utilities/octree.h"
#include "../game_manager.h"
#include "../get.h"
#include "../scene_graph/scene_graph.h"
#include "../../components/rendering/renderable.h"
#include "../../components/rendering/camera.h"
#include "../../components/rendering/light.h"
#include "device.h"
#include "upload_buffer.h"
#include "descriptor_heap.h"
#include "root_signature.h"
#include "pipeline_state.h"
#include "constants_helper.h"
#include "material.h"
#include "shader.h"
#include "texture.h"
#include "graphics.h"

#define SWAP_CHAIN_BUFFER_COUNT 2

bool draw_goblin = true;

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	Renderer::Renderer() :
		pass_constants_(nullptr),
		material_constants_(nullptr),
		object_constants_(nullptr),
		debug_constants_(nullptr),
		num_object_cbs_(0),
		num_material_cbs_(0),
		pass_cb_descriptor_id_(0),
		vertex_shader_(nullptr),
		pixel_shader_(nullptr),
		debug_vertex_shader_(nullptr),
		debug_pixel_shader_(nullptr),
		camera_(nullptr),
		octree_(nullptr),
		is_rendering_(false)
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	Renderer::~Renderer()
	{
		FlushCommandQueue();

		SAFE_DELETE(pass_constants_);
		SAFE_DELETE(material_constants_);
		SAFE_DELETE(object_constants_);
		SAFE_DELETE(debug_constants_);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::Startup()
	{
		Window* output_window = Get::Manager()->GetWindow();

		main_window_handle_ = output_window->GetWindowHandle();
		output_window_ = output_window;

		CreateDevice();

		Graphics::Initialize();

		CreateCommandObjects();
		CreateDescriptorHeaps();
		CreateSwapChain();
		CreateDepthStencilView();
		CreateSamplerDescs();

		vertex_shader_ = Get::ResourceManager()->GetShader("../../shaders/test_shader.hlsl", Shader::ShaderTypeVertex);
		pixel_shader_ = Get::ResourceManager()->GetShader("../../shaders/test_shader.hlsl", Shader::ShaderTypePixel);

		debug_vertex_shader_ = Get::ResourceManager()->GetShader("../../shaders/debug_shader.hlsl", Shader::ShaderTypeVertex);
		debug_pixel_shader_ = Get::ResourceManager()->GetShader("../../shaders/debug_shader.hlsl", Shader::ShaderTypePixel);

		CreateFrameResources();

		CreateDefaultPSO();

		FlushCommandQueue();

		buffer_manager_.Create(swap_chain_.GetBufferWidth(), swap_chain_.GetBufferHeight());

		GraphicsPSO& pso = GraphicsPSO::Get("");
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateDevice()
	{
		device_.Create(Get::ConfigManager()->GetAdapters()[Get::Config().adapter]);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateCommandObjects()
	{
		Get::CommandManager()->Startup(device_);
		Get::CommandContextManager()->Startup();
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateSwapChain()
	{
		swap_chain_.Create(
			main_window_handle_,
			Get::ConfigManager()->GetRenderResolutions()[Get::Config().render_resolution].width,
			Get::ConfigManager()->GetRenderResolutions()[Get::Config().render_resolution].height,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			SWAP_CHAIN_BUFFER_COUNT,
			DXGI_SWAP_EFFECT_FLIP_DISCARD,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateDescriptorHeaps()
	{
		rtv_heap_.Create(			D3D12_DESCRIPTOR_HEAP_TYPE_RTV,			D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 4096U);
		dsv_heap_.Create(			D3D12_DESCRIPTOR_HEAP_TYPE_DSV,			D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 4096U);
		cbv_srv_uav_heap_.Create(	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 4096U);
		sampler_heap_.Create(		D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 1024U);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateDepthStencilView()
	{
		depth_buffer_.Create(L"SceneDepthBuffer", swap_chain_.GetBufferWidth(), swap_chain_.GetBufferHeight(), DXGI_FORMAT_D32_FLOAT);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateFrameResources()
	{
		pass_constants_ = new UploadBuffer<PassConstants>(1U, true);
		object_constants_ = new UploadBuffer<ObjectConstants>(1024U, true);
		material_constants_ = new UploadBuffer<MaterialConstants>(1024U, true);
		debug_constants_ = new UploadBuffer<DebugConstants>(1024U, true);

		D3D12_GPU_VIRTUAL_ADDRESS cb_address = pass_constants_->GetBuffer()->GetGPUVirtualAddress();
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc;
		cbv_desc.BufferLocation = cb_address;
		cbv_desc.SizeInBytes = (sizeof(PassConstants) + 255) & ~255;
		pass_cb_descriptor_id_ = cbv_srv_uav_heap_.CreateConstantBufferView(&cbv_desc);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateSamplerDescs()
	{
		sampler_descs_.push_back(CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP));
		sampler_descs_.push_back(CD3DX12_STATIC_SAMPLER_DESC(1, D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP));
		sampler_descs_.push_back(CD3DX12_STATIC_SAMPLER_DESC(2, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP));
		sampler_descs_.push_back(CD3DX12_STATIC_SAMPLER_DESC(3, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP));
		sampler_descs_.push_back(CD3DX12_STATIC_SAMPLER_DESC(4, D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0.0f, 8));
		sampler_descs_.push_back(CD3DX12_STATIC_SAMPLER_DESC(5, D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 0.0f, 8));
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateRootSignature()
	{
		/*RootSignatureDesc desc;
		desc.flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.static_samplers = sampler_descs_;
		desc.parameters.push_back(RootSignatureParameter(ROOT_SIGNATURE_PARAMETER_TYPE_CBV, 0));
		desc.parameters.push_back(RootSignatureParameter(ROOT_SIGNATURE_PARAMETER_TYPE_CBV, 1));
		desc.parameters.push_back(RootSignatureParameter(ROOT_SIGNATURE_PARAMETER_TYPE_CBV, 2));
		desc.parameters.push_back(RootSignatureParameter(ROOT_SIGNATURE_PARAMETER_TYPE_TEXTURE, 0));
		desc.parameters.push_back(RootSignatureParameter(ROOT_SIGNATURE_PARAMETER_TYPE_TEXTURE, 1));
		desc.parameters.push_back(RootSignatureParameter(ROOT_SIGNATURE_PARAMETER_TYPE_TEXTURE, 2));
		desc.parameters.push_back(RootSignatureParameter(ROOT_SIGNATURE_PARAMETER_TYPE_TEXTURE, 3));
		desc.parameters.push_back(RootSignatureParameter(ROOT_SIGNATURE_PARAMETER_TYPE_TEXTURE, 4));
		desc.parameters.push_back(RootSignatureParameter(ROOT_SIGNATURE_PARAMETER_TYPE_TEXTURE, 5));

		root_signature_ = new RootSignature(desc);*/
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateDefaultPSO()
	{
		GraphicsPSO& default_renderable_draw = GraphicsPSO::Get("default_renderable_draw");
		default_renderable_draw.SetRootSignature(Graphics::root_signature_default);
		default_renderable_draw.SetVertexShader(vertex_shader_->GetShaderByteCode());
		default_renderable_draw.SetPixelShader(pixel_shader_->GetShaderByteCode());
		default_renderable_draw.SetBlendState(Graphics::blend_state_traditional);
		default_renderable_draw.SetDepthStencilState(Graphics::depth_state_default);
		default_renderable_draw.SetRasterizerState(Graphics::rasterizer_default_cw);
		default_renderable_draw.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		default_renderable_draw.SetInputLayout(_countof(input_element_vertex_desc), input_element_vertex_desc);
		default_renderable_draw.SetSampleMask(0xFFFFFFFF);
		default_renderable_draw.SetRenderTargetFormat(swap_chain_.GetBackBuffer().GetFormat(), depth_buffer_.GetFormat());
		default_renderable_draw.Finalize();

		GraphicsPSO& default_debug = GraphicsPSO::Get("default_debug");
		default_debug.SetRootSignature(Graphics::root_signature_default);
		default_debug.SetVertexShader(debug_vertex_shader_->GetShaderByteCode());
		default_debug.SetPixelShader(debug_pixel_shader_->GetShaderByteCode());
		default_debug.SetBlendState(Graphics::blend_state_disabled);
		default_debug.SetDepthStencilState(Graphics::depth_state_read_write);
		default_debug.SetRasterizerState(Graphics::rasterizer_default);
		default_debug.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		default_debug.SetInputLayout(_countof(input_element_vertex_desc), input_element_vertex_desc);
		default_debug.SetSampleMask(0xFFFFFFFF);
		default_debug.SetRenderTargetFormat(swap_chain_.GetBackBuffer().GetFormat(), depth_buffer_.GetFormat());
		default_debug.Finalize();
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::Draw(Timer* timer)
	{	
		GraphicsContext& context = GraphicsContext::Begin(L"SceneRender");
		
		is_rendering_ = true;

		ID3D12DescriptorHeap* heaps[2] = { cbv_srv_uav_heap_.Get(), sampler_heap_.Get() };
		D3D12_DESCRIPTOR_HEAP_TYPE heap_types[2] = { D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER };

		context.SetDescriptorHeaps(2, heap_types, heaps);
		context.TransitionResource(swap_chain_.GetBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET);

		ProcessPendingRenderables();

		ConstantsHelper::UpdatePassConstants(pass_constants_, DirectX::XMFLOAT2((float)swap_chain_.GetBufferWidth(), (float)swap_chain_.GetBufferHeight()), timer, camera_, lights_);
		ConstantsHelper::UpdateMaterialConstants(material_constants_, materials_);

		context.SetRenderTarget(swap_chain_.GetBackBuffer().GetRTV(), depth_buffer_.GetDSV());
		context.ClearColor(swap_chain_.GetBackBuffer());
		context.ClearDepth(depth_buffer_);
		context.SetViewportAndScissor(0, 0, swap_chain_.GetBufferWidth(), swap_chain_.GetBufferHeight());

		std::vector<Renderable*> renderable_set;

		ComputeCulling(renderable_set);

		ConstantsHelper::UpdateObjectConstants(object_constants_, renderable_set, camera_);

		DrawRenderables(context, renderable_set);
		DrawDebugVolumes(context, debug_volumes_);

		context.TransitionResource(swap_chain_.GetBackBuffer(), D3D12_RESOURCE_STATE_PRESENT);

		HRESULT hr = device_.Get()->GetDeviceRemovedReason();
		if (hr != S_OK)
		{
			_com_error error(hr);
			std::cout << error.ErrorMessage() << std::endl;
			assert(false);
		}

		context.Finish(true);
		swap_chain_.Present(false);

		is_rendering_ = false;

		for (int i = static_cast<int>(debug_volumes_.size()) - 1; i >= 0; i--)
		{
			delete debug_volumes_[i].mesh;

			debug_volumes_.erase(debug_volumes_.begin() + i);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::FlushCommandQueue()
	{
		Get::CommandManager()->WaitForIdleGPU();
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::ComputeCulling(std::vector<Renderable*>& out_renderables)
	{
		DirectX::XMMATRIX inv_view = camera_->GetInvView();
		DirectX::BoundingFrustum frustum;
		camera_->GetViewFrustum().Transform(frustum, inv_view);

		out_renderables.resize(0);

		if (Get::Config().frustum_culling)
		{
			Octree::PlaneFrustum frus;
			frustum.GetPlanes(&frus.near_plane, &frus.far_plane, &frus.right_plane, &frus.left_plane, &frus.top_plane, &frus.bottom_plane);
			octree_->GetIntersectingObjects(frus, out_renderables);
		}
		else
		{
			out_renderables = renderables_;
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::DrawRenderables(GraphicsContext& context, std::vector<Renderable*>& renderables)
	{
		context.SetRootSignature(Graphics::root_signature_default);
		context.SetPipelineState(GraphicsPSO::Get("default_renderable_draw"));

		context.SetConstantBuffer(1, pass_constants_->GetAddressById(0));

		for (int i = 0; i < static_cast<int>(renderables.size()); i++)
		{
			context.SetConstantBuffer(0, object_constants_->GetAddressById(renderables[i]->GetRendererObjectCBId()));
			context.SetConstantBuffer(2, material_constants_->GetAddressById(renderables[i]->GetMaterial()->renderer_material_cb_id));

			Material* mat = renderables[i]->GetMaterial();

			if (mat != nullptr)
			{
				if (mat->use_emissive_map == true)
				{
					context.SetDescriptorTable(3, cbv_srv_uav_heap_.GetGPUDescriptorById(mat->emissive_map->GetSRV()));
				}

				if (mat->use_ambient_map == true)
				{
					context.SetDescriptorTable(4, cbv_srv_uav_heap_.GetGPUDescriptorById(mat->ambient_map->GetSRV()));
				}

				if (mat->use_diffuse_map == true)
				{
					context.SetDescriptorTable(5, cbv_srv_uav_heap_.GetGPUDescriptorById(mat->diffuse_map->GetSRV()));
				}

				if (mat->use_specular_map == true)
				{
					context.SetDescriptorTable(6, cbv_srv_uav_heap_.GetGPUDescriptorById(mat->specular_map->GetSRV()));
				}

				if (mat->use_shininess_map == true)
				{
					context.SetDescriptorTable(7, cbv_srv_uav_heap_.GetGPUDescriptorById(mat->shininess_map->GetSRV()));
				}

				if (mat->use_normal_map == true)
				{
					context.SetDescriptorTable(8, cbv_srv_uav_heap_.GetGPUDescriptorById(mat->normal_map->GetSRV()));
				}
			}

			renderables[i]->GetMesh()->Draw(context);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::DrawDebugVolumes(GraphicsContext& context, std::vector<DebugVolume>& debug_volumes)
	{
		context.SetRootSignature(Graphics::root_signature_default);
		context.SetPipelineState(GraphicsPSO::Get("default_debug"));
		
		for (int i = 0; i < static_cast<int>(debug_volumes.size()); i++)
		{
			DebugVolume& current = debug_volumes[i];

			current.mesh->Draw(context);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::ProcessPendingRenderables()
	{
		bool renderables_updated = false;

		while (!renderables_to_remove_.empty())
		{
			renderables_updated = true;

			Renderable* to_remove = renderables_to_remove_.front();
			renderables_to_remove_.pop();

			for (int i = 0; i < renderables_.size(); i++)
			{
				if (renderables_[i] == to_remove)
				{
					renderables_.erase(renderables_.begin() + i);
					break;
				}
			}
		}

		UINT obj_cb_byte_size = (sizeof(ObjectConstants) + 255) & ~255;
		while (!renderables_to_add_.empty())
		{
			renderables_updated = true;

			Renderable* to_add = renderables_to_add_.front();
			renderables_to_add_.pop();

			renderables_.push_back(to_add);

			num_object_cbs_++;

			D3D12_CONSTANT_BUFFER_VIEW_DESC obj_cbv_desc;

			obj_cbv_desc.BufferLocation = object_constants_->GetAddressById(num_object_cbs_ - 1);
			obj_cbv_desc.SizeInBytes = obj_cb_byte_size;

			to_add->SetRendererObjectCBId(num_object_cbs_ - 1);
			to_add->SetRendererObjectDescriptorId(cbv_srv_uav_heap_.CreateConstantBufferView(&obj_cbv_desc));

			// material cb
			auto find = std::find(std::begin(materials_), std::end(materials_), to_add->GetMaterial());

			if (find == materials_.end())
			{
				Material* mat_to_add = to_add->GetMaterial();

				materials_.push_back(mat_to_add);

				num_material_cbs_++;

				D3D12_CONSTANT_BUFFER_VIEW_DESC mat_cbv_desc;

				mat_cbv_desc.BufferLocation = material_constants_->GetAddressById(num_material_cbs_ - 1);
				mat_cbv_desc.SizeInBytes = obj_cb_byte_size;

				mat_to_add->renderer_material_cb_id = num_material_cbs_ - 1;
				mat_to_add->renderer_material_descriptor_id = cbv_srv_uav_heap_.CreateConstantBufferView(&mat_cbv_desc);
			}
		}

		if (renderables_updated == true)
		{
			BuildOctree();
		}
	}

	//------------------------------------------------------------------------------------------------------
	Device& Renderer::GetDevice()
	{
		return device_;
	}

	//------------------------------------------------------------------------------------------------------
	const Device& Renderer::GetDevice() const
	{
		return device_;
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::AddRenderable(Renderable* renderable)
	{
		renderables_to_add_.push(renderable);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::DestroyRenderable(Renderable* renderable)
	{
		renderables_to_remove_.push(renderable);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::DestroyRenderableById(int id)
	{
		renderables_to_remove_.push(renderables_[id]);
	}

	//------------------------------------------------------------------------------------------------------
	Renderable* Renderer::GetRenderableById(int id)
	{
		return renderables_[id];
	}

	//------------------------------------------------------------------------------------------------------
	int Renderer::GetIdOfRenderable(Renderable* renderable)
	{
		for (int i = 0; i < renderables_.size(); i++)
		{
			if (renderables_[i] == renderable)
			{
				return i;
			}
		}

		return -1;
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::SetCamera(Camera* camera)
	{
		camera_ = camera;
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::BuildOctree()
	{
		if (octree_ != nullptr)
			delete octree_;

		DirectX::BoundingBox region(Vector3(), Vector3(150.0f, 150.0f, 150.0f));
		octree_ = new Octree(region, renderables_);
		octree_->BuildTree();
	}
	
	//------------------------------------------------------------------------------------------------------
	void Renderer::RenderDebugVolume(const DebugVolume& volume)
	{
		debug_volumes_.push_back(volume);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::AddLight(Light* light)
	{
		lights_.push_back(light);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::DestroyLight(Light* light)
	{
		for (int i = 0; i < lights_.size(); i++)
		{
			if (lights_[i] == light)
			{
				lights_.erase(lights_.begin() + i);
				return;
			}
		}
	}
}
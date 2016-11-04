#include "game_manager.h"

#include "win32/window.h"
#include "input/input_manager.h"
#include "rendering/renderer.h"
#include "rendering/command_manager.h"
#include "rendering/command_context_manager.h"
#include "utilities/timer.h"
#include "memory/memory_manager.h"
#include "scene_graph/component_manager.h"
#include "scene_graph/scene_graph.h"
#include "resources/resource_manager.h"
#include "resources/fbx_loader.h"
#include "physics/physics_manager.h"
#include "get.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	GameManager* GameManager::Create(size_t memory_size)
	{
		MemoryManager* memory_manager = new MemoryManager(memory_size);
		FreeListAllocator* own_allocator = memory_manager->GetNewAllocator<FreeListAllocator>(100000000);
		GameManager* game_manager = own_allocator->New<GameManager>(memory_manager, own_allocator);
		return game_manager;
	}

	//------------------------------------------------------------------------------------------------------
	void GameManager::Delete(GameManager* game_manager)
	{
		FreeListAllocator* own_allocator = game_manager->own_allocator_;
		MemoryManager* memory_manager = game_manager->memory_manager_;
		own_allocator->Delete(game_manager);
		memory_manager->DeleteAllocator(own_allocator);
		delete memory_manager;
	}

	//------------------------------------------------------------------------------------------------------
	GameManager::GameManager(MemoryManager* memory_manager, FreeListAllocator* own_allocator) :
		running_(true), 
		memory_manager_(memory_manager), 
		own_allocator_(own_allocator),
		subsystem_allocator_(nullptr)
	{
		Get::Create(this);
	}

	//------------------------------------------------------------------------------------------------------
	GameManager::~GameManager()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void GameManager::Startup(const std::string& name, int width, int height)
	{
		subsystem_allocator_		= memory_manager_->GetNewAllocator<StackAllocator>(10000);
		
		config_manager_				= subsystem_allocator_->New<ConfigManager>();
		window_						= subsystem_allocator_->New<Window>(name, config_manager_->GetWindowResolutions()[config_manager_->GetConfig().window_resolution].width, config_manager_->GetWindowResolutions()[config_manager_->GetConfig().window_resolution].height, this);
		input_manager_				= subsystem_allocator_->New<InputManager>(window_, this);
		resource_manager_			= subsystem_allocator_->New<ResourceManager>(this, 1000000);
		
		renderer_					= subsystem_allocator_->New<Renderer>();
		command_manager_			= subsystem_allocator_->New<CommandManager>();
		command_context_manager_	= subsystem_allocator_->New<CommandContextManager>();
		renderer_->Startup();

		timer_						= subsystem_allocator_->New<Timer>();
		component_manager_			= subsystem_allocator_->New<ComponentManager>();
		scene_						= subsystem_allocator_->New<Scene>(this, 10000000);
		physics_manager_			= PhysicsManager::Create(10000000);
	}

	//------------------------------------------------------------------------------------------------------
	void GameManager::LoadScene(const std::string& path_to_scene_fbx)
	{
		FBXLoader::LoadFBXScene(path_to_scene_fbx, scene_, true);
	}

	//------------------------------------------------------------------------------------------------------
	void GameManager::MainLoop()
	{
		component_manager_->Start();
		timer_->Reset();
		while (running_)
		{
			timer_->UpdateTimer();
			window_->ProcessMessages();
			component_manager_->UpdateBeforePhysics();
			physics_manager_->Update();
			component_manager_->UpdateAfterPhysics();
			input_manager_->Update();
			component_manager_->Update();
			renderer_->Draw(timer_);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void GameManager::StopRunning()
	{
		running_ = false;
	}

	//------------------------------------------------------------------------------------------------------
	void GameManager::ShutDown()
	{
		PhysicsManager::Delete(physics_manager_);
		subsystem_allocator_->Delete((Scene*)scene_);
		subsystem_allocator_->Delete(component_manager_);
		subsystem_allocator_->Delete(timer_);
		subsystem_allocator_->Delete(command_context_manager_);
		subsystem_allocator_->Delete(command_manager_);
		subsystem_allocator_->Delete(renderer_);
		subsystem_allocator_->Delete(resource_manager_);
		subsystem_allocator_->Delete(input_manager_);
		subsystem_allocator_->Delete(window_);
		subsystem_allocator_->Delete(config_manager_);

		memory_manager_->DeleteAllocator(subsystem_allocator_);
	}

	//------------------------------------------------------------------------------------------------------
	double GameManager::GetDeltaT()
	{
		return timer_->GetDeltaT();
	}

	//------------------------------------------------------------------------------------------------------
	double GameManager::GetTimeSinceStartup()
	{
		return timer_->GetTimeSinceStartup();
	}
	
	//------------------------------------------------------------------------------------------------------
	ConfigManager* GameManager::GetConfigManager()
	{
		return config_manager_;
	}
}
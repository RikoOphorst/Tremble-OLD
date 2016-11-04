#pragma once
#define _WINSOCK2API_
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <Windows.h>
#undef _WINSOCK2API_
#undef _WINSOCKAPI_
#include <string>
#include <functional>
#include "utilities\utilities.h"
#include "config/config_manager.h"

#include "PxPhysicsAPI.h"

namespace engine
{
	class ResourceManager;
	class FreeListAllocator;
	class StackAllocator;
	class MemoryManager;
	class Window;
	class InputManager;
	class ComponentManager;
	class Renderer;
	class CommandManager;
	class CommandContextManager;
	class Timer;
	class SGNode;
	class Scene;
	class PhysicsManager;

	/** 
	* @class engine::GameManager
	* @brief This is a main game manager class, that holds a game's instance
	* @author Anton Gavrilov
	*/
	class GameManager
	{
		friend class SGNode;
		friend class PhysicsRigidbodyComponent;
	public:
		/**
		* @brief GameManager constructor
		* @param[in] memory_manager Memory manager, that manages allocators
		* @param[in] own_allocator Allocator, that is used to hold all the subsystems of the game manager
		*/
		GameManager(MemoryManager* memory_manager, FreeListAllocator* own_allocator);

		/**
		* @brief GameManager destructor
		* @pre GameManager::ShutDown()
		*/
		~GameManager();

		/*
		* @brief Game manager factory design delete function. Only this should be used for game manager deletion
		* @pre GameManager::ShutDown()
		* @param[in] game_manager Game manager that you want to delete
		*/
		static void Delete(GameManager* game_manager);
		/*
		* @brief Game manager factory design create function. Only this should be used for game manager instantiation
		* @param[in] memory_size Memory size that should be allocated by the overall memory manager inside of game manager
		*/
		static GameManager* Create(size_t memory_size);
		
		void Startup(const std::string& name, int width, int height);
		
		void LoadScene(const std::string& path_to_scene_fbx);
		void MainLoop(); //!< A general engine update loop

		void StopRunning(); //!< shutdown the game manager after the current frame @see ShutDown()
		void ShutDown(); //!< Shutdown function of the engine. Here we properly shutdown everything and release all the resources that game manager holds @see StopRunning()

		ResourceManager* GetResourceManager() { return resource_manager_; } //!< Get the resouce manager
		MemoryManager* GetMemoryManager() { return memory_manager_; } //!< Get the game's memory manager in order to get new allocators
		Renderer* GetRenderer() { return renderer_; } //!< Get the game's renderer
		CommandManager* GetCommandManager() { return command_manager_; }
		CommandContextManager* GetCommandContextManager() { return command_context_manager_; }
		SGNode* GetScene() { return scene_; } //!< Get the root scene graph node of the scene
		PhysicsManager* GetPhysicsManager() { return physics_manager_; } //!< Get the physics manager of this game manager
		ComponentManager* GetComponentManager() { return component_manager_; } //!< Get the component manager of this game manager

		InputManager* GetInputManager() { return input_manager_; } //!< Get input manager tied with this game manager
		Window* GetWindow() { return window_; } //!< Get window of this game manager
		double GetDeltaT(); //!< Get time, that it took to run the last frame
		double GetTimeSinceStartup(); //!< Get time passed from startup to the end of the last frame
		ConfigManager* GetConfigManager(); //!< Get the config manager

	private:
		Timer* timer_; //!< Takes care of delta time
		bool running_; //!< Is the game running? @see StopRunning()
		FreeListAllocator* own_allocator_; //!< Allocator, that is used to allocate resources that are in use by the game manager itself
		StackAllocator* subsystem_allocator_; //!< Stack allocation strategy for allocating subsystems

		ConfigManager* config_manager_; //!< Manages the game config
		MemoryManager* memory_manager_; //!< Manages all the memory, used by the game
		InputManager* input_manager_; //!< Manages the input in the game
		ComponentManager* component_manager_; //!< Runs functions on all components. Components themselves are owned by the scene
		ResourceManager* resource_manager_; //!< Owns all the resources in the game (meshes, textures)
		PhysicsManager* physics_manager_; //!< Manages physics in the game(currently physx, later may be a different library)
		SGNode* scene_; //!< Scene graph root node

		Window* window_; //!< The window the game is utilizing
		Renderer* renderer_; //!< The renderer that draws everything onto the window
		CommandManager* command_manager_; // Manages commands send to the GPU
		CommandContextManager* command_context_manager_; // Manages all command contexts in the application
	};
}
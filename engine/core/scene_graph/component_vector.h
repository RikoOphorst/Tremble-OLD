#pragma once
#include <vector>
#include "component.h"
#include "../utilities/utilities.h"

namespace engine
{
	template <typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>	>
	class VectorComponent;


	/**
	* @class BaseComponentVector
	* @brief Interface for ComponentVector, and provides ability to store pointers to ComponentVector 
	*/
	class BaseComponentVector
	{
	public:
		virtual void Start() = 0; //!< Calls Awake() on all components if they have a void Awake()
		virtual void Update() = 0; //!< Calls Update() on all components if they have a void Update()
		virtual void UpdateBeforePhysics() = 0; //!< Calls UpdateBeforePhysics() on all components if they have a void Awake()
		virtual void UpdateAfterPhysics() = 0; //!< Calls UpdateAfterPhysics() on all components if they have a void Awake()
		virtual void RemoveComponent(Component* component) = 0;
	};

	/**
	* @class engine::ComponentVector
	* @brief Templated class of vector of components. Provides funcions to call certain functions (Update, Awake...) on all the components in the vector
	*
	* Accessor functions are implemented in such a way that if component has, for example, a function void Update() implemented, vector calls for Update() in all components
	* In other scenarios, Update() is just an empty function
	* @author Anton Gavrilov
	*/
	template <
		typename T,
		typename = std::enable_if_t<std::is_base_of<Component, T>::value>
	>
	class ComponentVector : BaseComponentVector
	{
		HAS_MEM_FUNC(Start, has_start);
		HAS_MEM_FUNC(Update, has_update);
		HAS_MEM_FUNC(UpdateBeforePhysics, has_before_physics);
		HAS_MEM_FUNC(UpdateAfterPhysics, has_after_physics);
	public:

		/**
		* @brief Add a component to the components vector. That means that the component will get called on update, awake, etc 
		* @param component Pointer to a component, that you want to add to the vector
		*/
		void AddComponent(T* component)
		{
			components_.push_back(component);
		}

		/**
		* @brief Remove component from the components vector. 
		* @param component Component that you wish to remove from the vector
		*/
		void RemoveComponent(Component* component) override
		{
			T* find = static_cast<T*>(component);
			std::vector<T*>::iterator found_comp = std::find(components_.begin(), components_.end(), find);
			ASSERT(found_comp != components_.end() && "You are trying to delete a component that doesn't exist");
			components_.erase(found_comp);
		}
		
		/**
		* @brief Templated function, that exists only if T has void Update() implemented
		*
		* has_update<class, sign> has a static variable value = 1 if T has an Update() implemented.
		* enable_if enables this function only if has_update<>::value = 1
		*/
		template<typename T>
		std::enable_if_t<has_update<T, void(T::*)()>::value, void>
			Update()
		{
			for each (T* i in components_)
			{
				i->Update();
			}
		}

		/**
		* @brief empty function template, that exists only if T does not have void Update() implemented
		*
		* has_update<class, sign> has a static variable value = 0 if T does not have an Update() implemented.
		* enable_if enables this function only if has_update<>::value = 1
		*/
		template<typename T>
		std::enable_if_t<!has_update<T, void(T::*)()>::value, void>
			Update()
		{

		}

		/**
		* @brief calls Update on components if they have awake implemented
		*
		* Just calls a generated template function Update with a template argument T so that you can call Update() on a component vector without passing template arguments
		*/
		void Update()
		{
			Update<T>();
		}

		/**
		* @brief Templated Update function, that exists only if T has void UpdateBeforePhysics() implemented
		*/
		template<typename T>
		std::enable_if_t<has_before_physics<T, void(T::*)()>::value, void>
			UpdateBeforePhysics()
		{
			for each (T* i in components_)
			{
				i->UpdateBeforePhysics();
			}
		}

		/**
		* @brief empty function template, that exists only if T does not have void UpdateBeforePhysics() implemented
		*/
		template<typename T>
		std::enable_if_t<!has_before_physics<T, void(T::*)()>::value, void>
			UpdateBeforePhysics()
		{

		}

		/**
		* @brief Calls UpdateBeforePhysics on all components if they have void UpdateBeforePhysics() implemented
		*/
		void UpdateBeforePhysics()
		{
			UpdateBeforePhysics<T>();
		}


		/**
		* @brief Templated Update function, that exists only if T has void UpdateBeforePhysics() implemented
		*/
		template<typename T>
		std::enable_if_t<has_after_physics<T, void(T::*)()>::value, void>
			UpdateAfterPhysics()
		{
			for each (T* i in components_)
			{
				i->UpdateAfterPhysics();
			}
		}

		/**
		* @brief empty function template, that exists only if T does not have void UpdateBeforePhysics() implemented
		*/
		template<typename T>
		std::enable_if_t<!has_after_physics<T, void(T::*)()>::value, void>
			UpdateAfterPhysics()
		{

		}

		/**
		* @brief Calls UpdateBeforePhysics on all components if they have void UpdateBeforePhysics() implemented
		*/
		void UpdateAfterPhysics()
		{
			UpdateAfterPhysics<T>();
		}

		/**
		* @brief Templated Update function, that exists only if T has void Start() implemented
		*/
		template<typename T>
		std::enable_if_t<has_start<T, void(T::*)()>::value, void>
			Start()
		{
			for each (T* i in components_)
			{
				i->Start();
			}
		}

		/**
		* @brief empty function template, that exists only if T does not have void UpdateBeforePhysics() implemented
		*/
		template<typename T>
		std::enable_if_t<!has_start<T, void(T::*)()>::value, void>
			Start()
		{

		}

		/**
		* @brief Calls UpdateBeforePhysics on all components if they have void UpdateBeforePhysics() implemented
		*/
		void Start()
		{
			Start<T>();
		}

	private:
		std::vector<T*> components_; //!< Component pointers, stored in this ComponentVector
	};
}
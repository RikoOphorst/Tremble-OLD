#pragma once

#include "pch.h"
#include "../math/math.h"
#include "../../core/get.h"

namespace engine
{
	class SGNode;

	/**
	* @class engine::Component
	* @brief Class, that all components have to inherit. 
	*/
	class Component
	{
	public:
		/**
		* @brief Component default constructor
		*/
		Component();
		/**
		* @brief Component intialization function
		* @param node Node, that this component is attached to
		* @param type_info Type of this component
		*/
		void Init(SGNode* node, std::type_index component_type);
		virtual void Shutdown() {} //!< Component shutdown function. Automatically gets called before the destruction

		std::type_index GetType() { return type_; } //!< Get type info if this component
		SGNode* GetNode() { return node_; } //!< Get a scene graph node of this component. Also used to gain access to game_manager

		const Vector3& GetPosition() const;
		const Quaternion& GetRotationQuaternion() const;
		const Vector3& GetScale() const;
		void SetPosition(const Vector3& position);
		void SetRotationQuaternion(const Quaternion& quaternion_rotation);
		void SetScale(const Vector3& scale);
	private:
		std::type_index type_; //!< type_info of the component. Used to store pointers to the base component class, and cast to a derived component class
		SGNode* node_; //!< Scene graph node of this component.
	};
}
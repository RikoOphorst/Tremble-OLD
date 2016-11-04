#pragma once

#include "pch.h"
#include "../game_manager.h"
#include "component_manager.h"
#include "../../components/rendering/renderable.h"
#include "../rendering/renderer.h"
#include "../memory/memory_includes.h"
#include "../math/math.h"
#include "../get.h"
#include "../utilities/utilities.h"

#define CHECK_THIS_DYNAMIC()	\
{	\
	if (is_static_ == true)	\
	{	\
		_CrtDbgBreak();	\
	}	\
}	\

namespace engine
{
	HAS_MEM_FUNC(Awake, has_awake);

	class Component;
	class GameManager;
	class FreeListAllocator;

	/**
	* @class SGNode
	* @brief A scene graph node. Has distinction between a static and dynamic node.
	*
	* A static node is not meant to be moved, rotated, or scaled. 
	* @author Anton Gavrilov
	*/
	class SGNode
	{

		friend class FBXLoader;
	public:
		SGNode(SGNode* parent); //!< Creates a dynamic scene graph node
		/**
		* @brief Construct a scene graph node with with an offset. The only way to construct a static node
		*/
		SGNode(SGNode* parent, bool is_static, const Vector3& position = Vector3(0.0f, 0.0f, 0.0f), const Quaternion& rotation = Quaternion(), const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f));
		
		void Destroy(); //!< Destroy this node, everything beneath it (components and nodes) and remove a pointer to this from the parent

		bool IsStatic() { return is_static_; } //!< Is this node static?
		bool IsDynamic() { return !is_static_; } //!< Is this node dynamic? (opposite of IsStatic)

		void SetPosition(const Vector3& position); //!< Set local position of this node. Can be used only with dynamic nodes. Marks world transform dirty. 
		void Move(const Vector3& movement); //!< Move the position of this node. Can be used only with dynamic nodes. Marks world transform dirty. 
		
		void SetRotationQuaternion(const Quaternion& rotation_quaternion); //!< Set local rotation of this node with a quaternion. Can be used only with dynamic nodes. Marks world transform dirty. 
		void SetRotationRadians(const Vector3& rotation_radians); //!< Set local rotation of this node with a vector, containing rotations in radians(x,y,z). Can be used only with dynamic nodes. Marks world transform dirty. 
		void SetRotationDegrees(const Vector3& rotation_degrees); //!< Set local rotation of this node with a vector, containing rotations in degrees(x,y,z). Can be used only with dynamic nodes. Marks world transform dirty. 
		
		void SetScale(const Vector3& scale); //!< Set local scale of this node. Can be used only with dynamic nodes. Marks world transform dirty. 

		void RotateQuaternion(const Quaternion& rotation);
		void RotateDegrees(const Vector3& rotation_degrees);
		void RotateRadians(const Vector3& rotation_radians);

		void RotateXRadians(Scalar radians_to_rotate); //!< Rotate this object around X axis by a certain angle around the x axis in radians) @param[in] local_axis Should the axis be the local axis of this node? 
		void RotateYRadians(Scalar radians_to_rotate); //!< Rotate this object around Y axis by a certain angle around the y axis in radians) @param[in] local_axis Should the axis be the local axis of this node? 
		void RotateZRadians(Scalar radians_to_rotate); //!< Rotate this object around Z axis by a certain angle around the z axis in radians) @param[in] local_axis Should the axis be the local axis of this node? 

		void RotateXDegrees(Scalar degrees_to_rotate); //!< Rotate this object around X axis by a certain angle around the world x axis in degrees
		void RotateYDegrees(Scalar degrees_to_rotate); //!< Rotate this object around Y axis by a certain angle around the world y axis in degrees
		void RotateZDegrees(Scalar degrees_to_rotate); //!< Rotate this object around Z axis by a certain angle around the world z axis in degrees

		const Vector3 GetForwardVectorWorld(); //!< Get a vector pointing forward of this node in world space (0, 0, 1 in case of no rotation)
		const Vector3 GetRightVectorWorld(); //!< Get a vector pointing right of this node in world space (1, 0, 0 in case of no rotation)
		const Vector3 GetUpVectorWorld(); //!< Get a vector pointing right of this node in world space (1, 0, 0 in case of no rotation)

		const Vector3& GetPosition() { return position_; } //!< Get local position of this node
		const Vector3& GetWorldPosition(); //!< Get world position of this node. Updates world transform matrix

		const Quaternion& GetRotationQuaternion() { return rotation_; } //!< Get local quaternion rotation of this node 
		const Vector3 GetRotationRadians(); //!< Get local rotation of this node represented by a vector, containing angles in radians(x, y, z)
		const Vector3 GetRotationDegrees(); //!< Get local rotation of this node represented by a vector, containing angles in degrees(x, y, z)

		const Quaternion GetWorldRotationQuaternion(); //!< Get world quaternion rotation of this node 
		const Vector3 GetWorldRotationRadians(); //!< Get local rotation of this node represented by a vector, containing angles in radians(x, y, z)
		const Vector3 GetWorldRotationDegrees(); //!< Get local rotation of this node represented by a vector, containing angles in degrees(x, y, z)

		const Vector3& GetScale() { return scale_; } //!< Get local scale of this node
		const Vector3& GetWorldScale();

		const DirectX::XMMATRIX& GetWorldTransform(); //!< Get world transform matrix of this node

		SGNode* GetParent() { return parent_; } //!< Get parent of this node
		GameManager* GetGameManager() { return game_manager_; } //<! Get the game manager

		SGNode* AddChild(bool create_static = false, Vector3 position = Vector3(), Vector3 Rotation = Vector3(), Vector3 scale = Vector3(1, 1, 1));

		template<typename T>
		std::enable_if_t<!has_awake<T, void(T::*)()>::value, T*>
		AddComponent(); //!< Creates a component and adds it to the components vector of pointers + to a component manager

		template<typename T, typename... Args>
		T* AddComponent(Args... args); //!< Creates a component and adds it to the components vector of pointers + to a component manager

		template <class T>
		void RemoveComponent(T* component); //!< Removes a component from the component manager and deletes it
		void RemoveComponents(); //!< Deletes all components of this node and removes them from the components vector.

		template <class T>
		T* FindComponent();

		template <class T>
		T* FindComponentInChildren();
	protected:
		SGNode(GameManager* game_manager); //!< Constuctor, only called by scene. To construct a node by yourself, call one of the other two
		bool is_static_; //!< Is this node static?

		SGNode* parent_; //!< This node's parent
		std::vector<SGNode*> children_; //!< A vector of children, that this node owns

		//local transform values
		Vector3 position_; //!< Local position
		Quaternion rotation_; //!< Local rotation
		Vector3 scale_; //!< Local scale

		bool world_position_updated_;
		Vector3 world_position_;
		inline void InvalidateWorldPosition_();
		inline void UpdateWorldPosition_();

		bool world_rotation_updated_;
		Quaternion world_rotation_;
		inline void InvalidateWorldRotation_();
		inline void UpdateWorldRotation_();

		bool world_scale_updated_;
		Vector3 world_scale_;
		inline void InvalidateWorldScale_();
		inline void UpdateWorldScale_();

		DirectX::XMMATRIX world_transform_; //!< World transformation matrix
		bool world_transform_updated_; //!< Is the world transform matrix updated at this moment
		inline void InvalidateWorldTransform_(); //!< Mark world transform matrix of this node and it's children as dirty
		inline void UpdateWorldTransform_(); //!< Update this node's world transform 

		void DecoupleChild_(SGNode* new_child); //!< Add a new child pointer to children's vector. Called only from SGNode's constructor
		void CoupleChild_(SGNode* child_to_remove); //!< Remove a chid pointer from childrens_ vector

		void DestroyChildren_(); //!< Destroy children of this node.

		std::vector<Component*> components_; //!< Components of this node

		GameManager* game_manager_; //!< Pointer to the game_manager
		FreeListAllocator* sg_allocator_; //!< Pointer to the allocator, used by the scene graph
	};


	//------------------------------------------------------------------------------------------------------
	class Scene : public SGNode
	{
	public:
		Scene(GameManager* game_manager_, size_t memory_size);
		~Scene();
	};

	//------------------------------------------------------------------------------------------------------
	template<typename T, typename... Args>
	 T* SGNode::AddComponent(Args... args)
	{
		T* created = sg_allocator_->New<T>();
		created->Init(this, typeid(T));
		components_.push_back(created);
		game_manager_->component_manager_->AddComponent(created);
		created->Awake(args...);
		return created;
	}

	//------------------------------------------------------------------------------------------------------
	template<typename T>
	std::enable_if_t<!has_awake<T, void(T::*)()>::value, T*>
	SGNode::AddComponent()
	{
		T* created = sg_allocator_->New<T>();
		created->Init(this, typeid(T));
		components_.push_back(created);
		game_manager_->component_manager_->AddComponent(created);
		return created;
	}

	//------------------------------------------------------------------------------------------------------
	template<>
	inline Renderable* SGNode::AddComponent<Renderable>()
	{
		Renderable* created = sg_allocator_->New<Renderable>();
		created->Init(this, typeid(Renderable));
		components_.push_back(created);
		game_manager_->component_manager_->AddComponent(created);
		Get::Renderer()->AddRenderable(created);
		return created;
	}

	//------------------------------------------------------------------------------------------------------
	template<class T>
	inline void SGNode::RemoveComponent(T* component)
	{
		std::vector<Component*>::iterator iter = std::find(components_.begin(), components_.end(), component);
		ASSERT(iter != components_.end());
		component->Shutdown();
		components_.erase(iter);
		game_manager_->component_manager_->RemoveComponent(component);
		sg_allocator_->Delete(component);
	}

	//------------------------------------------------------------------------------------------------------
	template<>
	inline void SGNode::RemoveComponent(Renderable* component)
	{
		std::vector<Component*>::iterator iter = std::find(components_.begin(), components_.end(), component);
		ASSERT(iter != components_.end());
		components_.erase(iter);
		game_manager_->component_manager_->RemoveComponent(component);
		Get::Renderer()->DestroyRenderable(component);
		sg_allocator_->Delete(component);
	}

	//------------------------------------------------------------------------------------------------------
	template<class T>
	inline T* SGNode::FindComponent()
	{
		for each(Component* i in components_)
		{
			if (i->GetType() == typeid(T))
			{
				return static_cast<T*>(i);
			}
		}
		return nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	template<class T>
	inline T* SGNode::FindComponentInChildren()
	{
		for each(Component* i in components_)
		{
			if (i->GetType() == typeid(T))
			{
				return static_cast<T*>(i);
			}
		}

		for each (SGNode* child in children_)
		{
			for each(Component* i in child->components_)
			{
				if (i->GetType() == typeid(T))
				{
					return static_cast<T*>(i);
				}
			}
		}

		return nullptr;
	}
}
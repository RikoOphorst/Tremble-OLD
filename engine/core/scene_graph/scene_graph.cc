#include "scene_graph.h"
#include "../utilities/debug.h"
#include "../game_manager.h"
#include "../memory/memory_includes.h"

namespace engine
{

	//------------------------------------------------------------------------------------------------------
	SGNode::SGNode(GameManager* game_manager)
		:parent_(nullptr), is_static_(true), game_manager_(game_manager)
	{
		position_ = DirectX::XMVectorReplicate(0.0f);
		rotation_ = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		scale_ = DirectX::XMVectorReplicate(1.0f);

		world_position_ = position_;
		world_rotation_ = rotation_;
		world_scale_ = scale_;

		world_position_updated_ = true;
		world_rotation_updated_ = true;
		world_scale_updated_ = true;

		world_transform_updated_ = true;

		world_transform_ =
			DirectX::XMMatrixScalingFromVector(scale_) *
			DirectX::XMMatrixRotationQuaternion(rotation_) *
			DirectX::XMMatrixTranslationFromVector(position_);
	}

	//------------------------------------------------------------------------------------------------------
	inline void SGNode::InvalidateWorldPosition_()
	{
		if (world_position_updated_)
		{
			world_transform_updated_ = false;
			world_position_updated_ = false;
			for each (SGNode* i in children_)
			{
				i->InvalidateWorldPosition_();
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	inline void SGNode::UpdateWorldPosition_()
	{
		world_position_ = parent_->GetWorldPosition() + parent_->GetWorldRotationQuaternion() * position_;
		world_position_updated_ = true;
	}

	//------------------------------------------------------------------------------------------------------
	inline void SGNode::InvalidateWorldRotation_()
	{
		CHECK_THIS_DYNAMIC();
		if (world_rotation_updated_)
		{
			world_transform_updated_ = false;
			world_rotation_updated_ = false;
			for each (SGNode* i in children_)
			{
				i->InvalidateWorldRotation_();
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	inline void SGNode::UpdateWorldRotation_()
	{
		CHECK_THIS_DYNAMIC();
		world_rotation_ = rotation_ * parent_->GetWorldRotationQuaternion();
		world_rotation_updated_ = true;
	}

	//------------------------------------------------------------------------------------------------------
	inline void SGNode::InvalidateWorldScale_()
	{
		CHECK_THIS_DYNAMIC();
		if (world_scale_updated_)
		{
			world_transform_updated_ = false;
			world_scale_updated_ = false;
			for each (SGNode* i in children_)
			{
				i->InvalidateWorldScale_();
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	inline void SGNode::UpdateWorldScale_()
	{
		CHECK_THIS_DYNAMIC();
		world_scale_ = parent_->GetWorldScale() * scale_;
		world_scale_updated_ = true;
	}

	//------------------------------------------------------------------------------------------------------
	SGNode::SGNode(SGNode* parent) : 
		parent_(parent), 
		is_static_(false)
	{
		position_ = DirectX::XMVectorReplicate(0.0f);
		rotation_ = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		scale_ = DirectX::XMVectorReplicate(1.0f);

		game_manager_ = parent->game_manager_;
		sg_allocator_ = parent->sg_allocator_;

		world_transform_updated_ = false;

		world_position_updated_ = false;
		world_rotation_updated_ = false;
		world_scale_updated_ = false;

		parent_->CoupleChild_(this);
	}

	//------------------------------------------------------------------------------------------------------
	SGNode::SGNode(SGNode * parent, bool is_static, const Vector3& position, const Quaternion& rotation, const Vector3& scale) :
		parent_(parent), 
		is_static_(false) //A little hack to be able to use UpdateWorldTransform()
	{
		sg_allocator_ = parent->sg_allocator_;
		game_manager_ = parent->game_manager_;

		world_transform_updated_ = false;

		world_position_updated_ = false;
		world_rotation_updated_ = false;
		world_scale_updated_ = false;

		parent_->CoupleChild_(this);

		position_ = position;
		rotation_ = rotation;
		scale_ = scale;

		//Multiplication takes order as parentWT * S * R * T
		UpdateWorldTransform_();

		is_static_ = is_static;

		//A parent of a static node can only be static (or nonexistent, in case of scene)
		ASSERT(is_static == false || parent_ == nullptr || parent->IsStatic() == true);
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::Destroy()
	{
		DestroyChildren_();
		RemoveComponents();
		parent_->DecoupleChild_(this);
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::DestroyChildren_()
	{
		size_t size = children_.size();
		for (int i = 0; i < size; i++)
		{
			children_.back()->DestroyChildren_();
			sg_allocator_->Delete(children_.back());
			children_.pop_back();
		}
		RemoveComponents();
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::SetPosition(const Vector3& position)
	{
		//Cannot move a static node
		CHECK_THIS_DYNAMIC();
		position_ = position;
		InvalidateWorldPosition_();
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::Move(const Vector3 & movement)
	{
		SetPosition(position_ + movement);
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::SetRotationQuaternion(const Quaternion& rotation_quaternion)
	{
		//Cannot rotate a static node
		CHECK_THIS_DYNAMIC();
		rotation_ = rotation_quaternion;
		InvalidateWorldRotation_();
		InvalidateWorldRotation_();
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::SetRotationRadians(const Vector3& rotation_radians)
	{
		CHECK_THIS_DYNAMIC();
		SetRotationQuaternion(Quaternion(rotation_radians.GetZ(), rotation_radians.GetX(), rotation_radians.GetY()));
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::SetRotationDegrees(const Vector3& rotation_degrees)
	{
		CHECK_THIS_DYNAMIC();
		SetRotationQuaternion(Quaternion(rotation_degrees.GetZ().ToRadians(), rotation_degrees.GetX().ToRadians(), rotation_degrees.GetY().ToRadians()));
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::SetScale(const Vector3& scale)
	{
		//Cannot scale a static node
		CHECK_THIS_DYNAMIC();
		scale_ = scale;
		InvalidateWorldScale_();
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RotateQuaternion(const Quaternion& rotation)
	{
		SetRotationQuaternion(rotation_ * rotation);
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RotateRadians(const Vector3& rotation_radians)
	{
		RotateQuaternion(Quaternion(rotation_ * Vector3(rotation_radians.GetX(), Scalar(0.0f), rotation_radians.GetZ())) * Quaternion(Vector3(0, 1, 0), rotation_radians.GetY()));
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RotateDegrees(const Vector3& rotation_degrees)
	{
		RotateRadians(Vector3(rotation_degrees.GetX().ToRadians(), rotation_degrees.GetY().ToRadians(), rotation_degrees.GetZ().ToRadians()));
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RotateXRadians(const Scalar radians_to_rotate)
	{
		RotateQuaternion(Quaternion(rotation_ * Vector3(1, 0, 0), radians_to_rotate));
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RotateYRadians(const Scalar radians_to_rotate)
	{
		RotateQuaternion(Quaternion(Vector3(0, 1, 0), radians_to_rotate));
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RotateZRadians(const Scalar radians_to_rotate)
	{
		RotateQuaternion(Quaternion(rotation_ * Vector3(0, 0, 1), radians_to_rotate));
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RotateXDegrees(const Scalar degrees_to_rotate)
	{
		RotateXRadians(degrees_to_rotate.ToRadians());
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RotateYDegrees(const Scalar degrees_to_rotate)
	{
		RotateYRadians(degrees_to_rotate.ToRadians());
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RotateZDegrees(const Scalar degrees_to_rotate)
	{
		RotateZRadians(degrees_to_rotate.ToRadians());
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3 SGNode::GetForwardVectorWorld()
	{
		return GetWorldRotationQuaternion() * Vector3(0, 0, 1);
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3 SGNode::GetRightVectorWorld()
	{
		return GetWorldRotationQuaternion() * Vector3(1, 0, 0);
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3 SGNode::GetUpVectorWorld()
	{
		return GetWorldRotationQuaternion() * Vector3(0, 1, 0);
	}

	//------------------------------------------------------------------------------------------------------
	const Quaternion SGNode::GetWorldRotationQuaternion()
	{
		if (!world_rotation_updated_)
		{
			UpdateWorldRotation_();
		}
		return world_rotation_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3 SGNode::GetWorldRotationRadians()
	{
		return GetWorldRotationQuaternion().ToEuler();
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3 SGNode::GetWorldRotationDegrees()
	{
		Vector3 rotation = GetWorldRotationRadians();
		rotation.SetX(rotation.GetX().ToDegrees());
		rotation.SetY(rotation.GetY().ToDegrees());
		rotation.SetZ(rotation.GetZ().ToDegrees());
		return rotation;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3 SGNode::GetRotationRadians()
	{
		return rotation_.ToEuler();
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3 SGNode::GetRotationDegrees()
	{
		Vector3 rotation = GetRotationRadians();
		rotation.SetX(rotation.GetX().ToDegrees());
		rotation.SetY(rotation.GetY().ToDegrees());
		rotation.SetZ(rotation.GetZ().ToDegrees());
		return rotation;
	}

	//------------------------------------------------------------------------------------------------------
	const DirectX::XMMATRIX& SGNode::GetWorldTransform()
	{
		if (world_transform_updated_ == false)
		{
			UpdateWorldTransform_();
		}
		return world_transform_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& SGNode::GetWorldPosition()
	{
		if (world_position_updated_ == false)
		{
			UpdateWorldPosition_();
		}
		return world_position_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& SGNode::GetWorldScale()
	{
		if (world_scale_updated_ == false)
		{
			UpdateWorldScale_();
		}
		return world_scale_;
	}

	//------------------------------------------------------------------------------------------------------
	SGNode* SGNode::AddChild(bool create_static, Vector3 position, Vector3 rotation, Vector3 scale)
	{
		return sg_allocator_->New<SGNode>(this, create_static, position, Quaternion(rotation), scale);
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::CoupleChild_(SGNode* new_child)
	{
		children_.push_back(new_child);
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::DecoupleChild_(SGNode * child_to_remove)
	{
		std::vector<SGNode*>::iterator found_child = std::find(children_.begin(), children_.end(), child_to_remove);
		ASSERT(found_child != children_.end());
		children_.erase(found_child);
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::InvalidateWorldTransform_()
	{
		//World transform of a static node doesn't make sense to be invalidated.
		//It gets constructed one time during initialization of a static node and stays like this all time
		CHECK_THIS_DYNAMIC();
		if (world_transform_updated_ == true)
		{
			world_transform_updated_ = false;
			for each (SGNode* i in children_)
			{
				i->InvalidateWorldTransform_();
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::UpdateWorldTransform_()
	{
		//World transform of a static node doesn't make sense to be updated.
		//It gets constructed one time during initialization of a static node and stays like this all time
		CHECK_THIS_DYNAMIC();

		world_transform_ =
			DirectX::XMMatrixScalingFromVector(GetWorldScale()) *
			DirectX::XMMatrixRotationQuaternion(GetWorldRotationQuaternion()) *
			DirectX::XMMatrixTranslationFromVector(GetWorldPosition());

		world_transform_updated_ = true;
	}

	//------------------------------------------------------------------------------------------------------
	void SGNode::RemoveComponents()
	{
		size_t size = components_.size();
		for (int i = 0; i < size; i++)
		{
			components_.back()->Shutdown();
			game_manager_->component_manager_->RemoveComponent(components_.back());
			sg_allocator_->Delete(components_.back());
			components_.pop_back();
		}
	}

	//------------------------------------------------------------------------------------------------------
	Scene::Scene(GameManager* game_manager, size_t memory_size)
		:SGNode(game_manager)
	{
		sg_allocator_ = game_manager_->GetMemoryManager()->GetNewAllocator<FreeListAllocator>(memory_size);
	}

	//------------------------------------------------------------------------------------------------------
	Scene::~Scene()
	{
		DestroyChildren_();
		game_manager_->GetMemoryManager()->DeleteAllocator(sg_allocator_);
	}


}
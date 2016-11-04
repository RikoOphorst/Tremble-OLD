#include "pch.h"
#include "component.h"

#include "scene_graph.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	Component::Component() :
		type_(typeid(nullptr))
	{

	}

	//------------------------------------------------------------------------------------------------------
	void Component::Init(SGNode* node, std::type_index component_type)
	{
		node_ = node;
		type_ = component_type;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Component::GetPosition() const
	{
		return node_->GetPosition();
	}

	//------------------------------------------------------------------------------------------------------
	const Quaternion& Component::GetRotationQuaternion() const
	{
		return node_->GetRotationQuaternion();
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Component::GetScale() const
	{
		return node_->GetScale();
	}

	//------------------------------------------------------------------------------------------------------
	void Component::SetPosition(const Vector3& position)
	{
		node_->SetPosition(position);
	}

	//------------------------------------------------------------------------------------------------------
	void Component::SetRotationQuaternion(const Quaternion& rotation)
	{
		node_->SetRotationQuaternion(rotation);
	}

	//------------------------------------------------------------------------------------------------------
	void Component::SetScale(const Vector3& scale)
	{
		node_->SetScale(scale);
	}
}
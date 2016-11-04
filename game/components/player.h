#pragma once
#include "pch.h"

namespace engine
{
	class Player : public Component
	{
	public:
		void Start()
		{
			character_controller_ = GetNode()->FindComponent<CharacterController>();
			if (character_controller_ == nullptr)
			{
				DLOG("Character controller not found for player");
			}
			Camera* camera_component = GetNode()->FindComponentInChildren<Camera>();
			camera_node_ = camera_component->GetNode();
			character_controller_->SetGravity(-20.0f);
			shoot_dynamics_ = GetNode()->FindComponentInChildren<RaycastShootDynamics>();
		}
		void Update()
		{
			if (Get::InputManager()->GetMousePressed(MOUSE_BUTTON_LEFT))
			{
				Vector3 direction = camera_node_->GetForwardVectorWorld();
				shoot_dynamics_->ShootTargets(camera_node_->GetWorldPosition() + direction * 0.8f, direction);
				camera_pitch -= 0.07f;
				comeback_angle_ += 0.05f;
			}

			float portion_of_angle = comeback_angle_ * 0.04;
			comeback_angle_ -= portion_of_angle;
			camera_pitch += portion_of_angle;

			float movement_speed_this_frame = movement_speed_;
			if (Get::InputManager()->GetKeyDown(TYPE_KEY_RIGHT_CONTROL) || Get::InputManager()->GetKeyDown(TYPE_KEY_RIGHT_SHIFT))
			{
				movement_speed_this_frame *= 2.5f;
			}

			DirectX::XMINT2 mouse_delta = Get::InputManager()->GetMouseDeltaPos();
			GetNode()->RotateYRadians(Scalar(horizontal_mouse_rotation_speed_ * mouse_delta.x));
			camera_pitch += Scalar(vertical_mouse_rotation_speed_ * mouse_delta.y);
			if (Get::InputManager()->GetKeyDown(TYPE_KEY_NUMPAD8))
			{
				camera_pitch += Scalar(-keys_rotation_speed_ * 100 * Get::DeltaT());
			}
			if (Get::InputManager()->GetKeyDown(TYPE_KEY_NUMPAD2))
			{
				camera_pitch += Scalar(keys_rotation_speed_ * 100 * Get::DeltaT());
			}
			//clamp camera to 90/-70
			camera_pitch = std::max((float)-1.22173f, std::min((float)camera_pitch, 1.5708f));
			camera_node_->SetRotationRadians(Vector3(camera_pitch, Scalar(0.0f), Scalar(0.0f)));
			if (Get::InputManager()->GetKeyDown(TYPE_KEY_NUMPAD4))
			{
				GetNode()->RotateYRadians(Scalar(-keys_rotation_speed_ * 100 * Get::DeltaT()));
			}
			if (Get::InputManager()->GetKeyDown(TYPE_KEY_NUMPAD6))
			{
				GetNode()->RotateYRadians(Scalar(keys_rotation_speed_ * 100 * Get::DeltaT()));
			}
			if (Get::InputManager()->GetKeyDown(TYPE_KEY_UP) || Get::InputManager()->GetKeyDown(TYPE_KEY_W))
			{
				character_controller_->Move(GetNode()->GetForwardVectorWorld() * movement_speed_this_frame);
			}
			if (Get::InputManager()->GetKeyDown(TYPE_KEY_LEFT) || Get::InputManager()->GetKeyDown(TYPE_KEY_A))
			{
				character_controller_->Move(-GetNode()->GetRightVectorWorld() * movement_speed_this_frame);
			}
			if (Get::InputManager()->GetKeyDown(TYPE_KEY_RIGHT) || Get::InputManager()->GetKeyDown(TYPE_KEY_D))
			{
				character_controller_->Move(GetNode()->GetRightVectorWorld() * movement_speed_this_frame);
			}
			if (Get::InputManager()->GetKeyDown(TYPE_KEY_DOWN) || Get::InputManager()->GetKeyDown(TYPE_KEY_S))
			{
				character_controller_->Move(-GetNode()->GetForwardVectorWorld() * movement_speed_this_frame);
			}
			if (Get::InputManager()->GetKeyPressed(TYPE_KEY_SPACE))
			{
				if (character_controller_->IsOnGround())
				{
					character_controller_->Jump(Scalar(7.0f));
				}
			}
		}
	private:
		float comeback_angle_ = 0;
		float vertical_angle_ = 0; //!< Vertical angle of the look of the player(in radians)
		Scalar camera_pitch = 0;
		float keys_rotation_speed_ = 0.02;
		float horizontal_mouse_rotation_speed_ = 0.005;
		float vertical_mouse_rotation_speed_ = 0.003;
		float movement_speed_ = 6;
		CharacterController* character_controller_;
		RaycastShootDynamics* shoot_dynamics_;
		SGNode* camera_node_;
	};
}
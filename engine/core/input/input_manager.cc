#include "input_manager.h"
#include "../../core/game_manager.h"
#include "../utilities/utilities.h"
#include "../win32/window.h"
#include "../get.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	InputManager::InputManager(Window* window, GameManager* game_manager)
		:window_(window), game_manager_(game_manager)
	{
		std::function<void(MSG message)> message = std::bind(&InputManager::ProcessWinMessage, this, std::placeholders::_1);
		window_->SetWindowInputListener(message);
	}

	//------------------------------------------------------------------------------------------------------
	void InputManager::ProcessWinMessage(MSG message)
	{
		switch (message.message)
		{
		case WM_KEYDOWN:
		{
			if (message.wParam == VK_ESCAPE)
			{
				game_manager_->StopRunning();
			}
			key_events_.push(KeyEvent{ KeyEvent::KEY_DOWN, static_cast<KEY_TYPE>(message.wParam) });
			break;
		}
		case WM_KEYUP:
		{
			key_events_.push(KeyEvent{ KeyEvent::KEY_UP, static_cast<KEY_TYPE>(message.wParam) });
			break;
		}
		case WM_LBUTTONDOWN:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_LEFT, MouseEvent::MOUSE_EVENT_DOWN, message.wParam, message.lParam });
			break;
		case WM_LBUTTONUP:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_LEFT, MouseEvent::MOUSE_EVENT_UP, message.wParam, message.lParam });
			break;
		case WM_LBUTTONDBLCLK:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_LEFT, MouseEvent::MOUSE_EVENT_DBL, message.wParam, message.lParam });
			break;
		case WM_RBUTTONDOWN:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_RIGHT, MouseEvent::MOUSE_EVENT_DOWN, message.wParam, message.lParam });
			break;
		case WM_RBUTTONUP:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_RIGHT, MouseEvent::MOUSE_EVENT_UP, message.wParam, message.lParam });
			break;
		case WM_RBUTTONDBLCLK:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_RIGHT, MouseEvent::MOUSE_EVENT_DBL, message.wParam, message.lParam });
			break;
		case WM_MBUTTONDOWN:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_MIDDLE, MouseEvent::MOUSE_EVENT_DOWN, message.wParam, message.lParam });
			break;
		case WM_MBUTTONUP:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_MIDDLE, MouseEvent::MOUSE_EVENT_UP, message.wParam, message.lParam });
			break;
		case WM_MBUTTONDBLCLK:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_MIDDLE, MouseEvent::MOUSE_EVENT_DBL, message.wParam, message.lParam });
			break;
		case WM_MOUSEMOVE:
			if (is_cursor_locked_)
			{
				DirectX::XMINT2 new_mouse_position = DirectX::XMINT2(static_cast<float>(((int)(short)LOWORD(message.lParam))), static_cast<float>(((int)(short)HIWORD(message.lParam))));
				if (new_mouse_position.x != window_center_local_.x || new_mouse_position.y != window_center_local_.y)
				{
					POINT new_center = window_center_point_;
					ClientToScreen(Get::Window()->GetWindowHandle(), &new_center);
					SetCursorPos(new_center.x, new_center.y);
				}
			}
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_NULL, MouseEvent::MOUSE_EVENT_MOVE, message.wParam, message.lParam });
			break;
		case WM_MOUSEWHEEL:
			mouse_events_.push(MouseEvent{ MOUSE_BUTTON_NULL, MouseEvent::MOUSE_EVENT_WHEEL, message.wParam, message.lParam });
			break;
		}
	}

	//------------------------------------------------------------------------------------------------------
	void InputManager::Update()
	{
		//Clear the per frame pressed and released values
		uint size = static_cast<uint>(key_states_.size());
		for (auto iter = key_states_.begin(); iter != key_states_.end(); iter++)
		{
			iter->second.pressed = false;
			iter->second.released = false;
		}

		size = static_cast<uint>(mouse_button_states_.size());
		for (auto iter = mouse_button_states_.begin(); iter != mouse_button_states_.end(); iter++)
		{
			iter->second.dbl = false;
			iter->second.pressed = false;
			iter->second.released = false;
		}

		//Clear mouse delta values
		mouse_state_.delta_position = DirectX::XMINT2(0.0f, 0.0f);
		mouse_state_.delta_wheel = 0.0f;

		//Process input events
		ProcessEvents();
	}

	//------------------------------------------------------------------------------------------------------
	void InputManager::HideCursor()
	{
		if (!is_cursor_hidden_)
		{
			ShowCursor(false);
		}
	}

	//------------------------------------------------------------------------------------------------------
	bool InputManager::isCursorHidden()
	{
		return is_cursor_hidden_;
	}

	//------------------------------------------------------------------------------------------------------
	void InputManager::UnHideCursor()
	{
		ShowCursor(true);
	}

	//------------------------------------------------------------------------------------------------------
	void InputManager::LockCursor()
	{
		int vertical_center = (Get::Window()->GetHeight() / 2);
		int horizontal_center = (Get::Window()->GetWidth() / 2);
		window_center_point_ = { horizontal_center, vertical_center };
		window_center_local_ = DirectX::XMINT2(horizontal_center, vertical_center);
		is_cursor_locked_ = true;
		int window_x, window_y;
		Get::Window()->GetPosition(window_x, window_y);
		RECT window_rect = RECT{ (LONG)window_x, (LONG)window_y, (LONG)window_x + (LONG)Get::Window()->GetWidth() - 1, (LONG)window_y + (LONG)Get::Window()->GetHeight() - 1 };
		ClipCursor(&window_rect);
	}

	//------------------------------------------------------------------------------------------------------
	bool InputManager::IsCursorLocked()
	{
		return is_cursor_locked_;
	}

	//------------------------------------------------------------------------------------------------------
	void InputManager::UnlockCursor()
	{
		is_cursor_locked_ = false;
		ClipCursor(NULL);
	}

	//------------------------------------------------------------------------------------------------------
	void InputManager::ProcessEvents()
	{
		//Process Key events
		uint size = static_cast<uint>(key_events_.size());
		for (int i = 0; i < static_cast<int>(size); i++)
		{
			KeyEvent& key_event = key_events_.front();
			if (key_event.event_type == KeyEvent::KEY_DOWN)
			{
				if (key_states_[key_event.key_type].down == true)
				{
					key_states_[key_event.key_type].repeated = true;
				}
				else
				{
					key_states_[key_event.key_type].pressed = true;
					key_states_[key_event.key_type].down = true;
				}
			}
			else //KEY_UP
			{
				key_states_[key_event.key_type].released = true;
				key_states_[key_event.key_type].down = false;
				key_states_[key_event.key_type].repeated = false;
			}
			key_events_.pop();
		}

		//Process Mouse Events
		size = static_cast<uint>(mouse_events_.size());
		for (int i = 0; i < static_cast<int>(size); i++)
		{
			MouseEvent& mouse_event = mouse_events_.front();

			switch (mouse_event.event_type)
			{
			case MouseEvent::MOUSE_EVENT_DOWN:
			{
				MouseButtonState& mouse_button_state = mouse_button_states_[mouse_event.button_type];
				mouse_button_state.pressed = true;
				mouse_button_state.down = true;
			}
			break;
			case MouseEvent::MOUSE_EVENT_UP:
			{
				MouseButtonState& mouse_button_state = mouse_button_states_[mouse_event.button_type];
				mouse_button_state.released = true;
				mouse_button_state.down = false;
			}
			break;
			case MouseEvent::MOUSE_EVENT_DBL:
			{
				MouseButtonState& mouse_button_state = mouse_button_states_[mouse_event.button_type];
				mouse_button_state.dbl = true;
			}
			break;
			case MouseEvent::MOUSE_EVENT_MOVE:
			{
				window_->GetWindowHandle();

				//The LOWORD HIWORD stuff is actually copied from <windowsx.h> just to not include the whole thing here
				DirectX::XMINT2 new_mouse_position = DirectX::XMINT2(static_cast<float>(((int)(short)LOWORD(mouse_event.lparam))), static_cast<float>(((int)(short)HIWORD(mouse_event.lparam))));

				mouse_state_.delta_position.x += new_mouse_position.x - mouse_state_.position.x;
				mouse_state_.delta_position.y += new_mouse_position.y - mouse_state_.position.y;

				if (is_cursor_locked_ == false)
				{
					mouse_state_.position = new_mouse_position;
				}
				else
				{
					mouse_state_.position = window_center_local_;
				}
			}
			break;
			case MouseEvent::MOUSE_EVENT_WHEEL:
			{
				mouse_state_.delta_wheel = static_cast<float>(GET_WHEEL_DELTA_WPARAM(mouse_event.wparam));
			}
			break;
			}
			mouse_events_.pop();
		}
	}

	//------------------------------------------------------------------------------------------------------
	bool InputManager::GetKeyDown(KEY_TYPE key)
	{
		return key_states_[key].down;
		//return key_states_[key].down;
	}

	//------------------------------------------------------------------------------------------------------
	bool InputManager::GetKeyPressed(KEY_TYPE key)
	{
		return key_states_[key].pressed;
	}

	//------------------------------------------------------------------------------------------------------
	bool InputManager::GetKeyReleased(KEY_TYPE key)
	{
		return key_states_[key].released;
	}

	//------------------------------------------------------------------------------------------------------
	bool InputManager::GetKeyRepeated(KEY_TYPE key)
	{
		return key_states_[key].repeated;
	}

	//------------------------------------------------------------------------------------------------------
	bool InputManager::GetMouseDown(MOUSE_BUTTON_TYPE key)
	{
		return mouse_button_states_[key].down;
	}

	//------------------------------------------------------------------------------------------------------
	bool InputManager::GetMousePressed(MOUSE_BUTTON_TYPE key)
	{
		return mouse_button_states_[key].pressed;
	}

	//------------------------------------------------------------------------------------------------------
	bool InputManager::GetMouseReleased(MOUSE_BUTTON_TYPE key)
	{
		return mouse_button_states_[key].released;
	}

	//------------------------------------------------------------------------------------------------------
	DirectX::XMINT2 InputManager::GetMousePos()
	{
		return mouse_state_.position;
	}

	//------------------------------------------------------------------------------------------------------
	DirectX::XMINT2 InputManager::GetMouseDeltaPos()
	{
		return mouse_state_.delta_position;
	}

	//------------------------------------------------------------------------------------------------------
	float InputManager::GetMouseDeltaWheel()
	{
		return mouse_state_.delta_wheel;
	}
}
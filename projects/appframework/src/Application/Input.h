#pragma once

#include "core.h"
#include "Maths.h"
#include "InputCodes.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

enum class InputState {
	None = 0,
	Pressed,
	Released
};

namespace Application {
	class AF_API InputManager {
	private:
		static std::unordered_map<KeyCode, InputState> m_sKeyCache;
		static std::unordered_map<MouseCode, InputState> m_sMouseButtonCache;

		static std::unordered_map<KeyCode, InputState> m_sLastKeyCache; // stores key pressed/released in the last frame
		static std::unordered_map<MouseCode, InputState> m_sLastMouseButtonCache; // stores mouse button pressed/released in the last frame

		static TVec2<double> m_sMousePosition;
		static TVec2<double> m_sLastMousePosition;
		static TVec2<double> m_sDeltaMousePosition;

	public:
		static void Init(void* window_handle);
		static void Update();

		// <--------------------- KEYBOARD ---------------------->

		static bool KeyDown(KeyCode key_code);
		static bool KeyPressed(KeyCode key_code);
		static bool KeyReleased(KeyCode key_code);

		// <--------------------- MOUSE ---------------------->

		static bool MouseDown(MouseCode mouse_code);
		static bool MousePressed(MouseCode mouse_code);
		static bool MouseReleased(MouseCode mouse_code);
	};
}
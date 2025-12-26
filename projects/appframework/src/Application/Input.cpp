
#include "afpch.h"
#include "Input.h"

namespace Application {
	std::unordered_map<KeyCode, InputState> InputManager::m_sKeyCache = {};
	std::unordered_map<MouseCode, InputState> InputManager::m_sMouseButtonCache = {};

	std::unordered_map<KeyCode, InputState> InputManager::m_sLastKeyCache = {};
	std::unordered_map<MouseCode, InputState> InputManager::m_sLastMouseButtonCache = {};

	TVec2<double> InputManager::m_sMousePosition = { 0.0, 0.0 };
	TVec2<double> InputManager::m_sLastMousePosition = { 0.0, 0.0 };
	TVec2<double> InputManager::m_sDeltaMousePosition = { 0.0, 0.0 };

	static int InputStateToGLFWBase(InputState is) {
		switch (is) {
		case InputState::None: return NULL; break;
		case InputState::Pressed: return GLFW_PRESS; break;
		case InputState::Released: return GLFW_RELEASE; break;
		}
	}

	void InputManager::Init(void* window_handle) {
		AF_ASSERT(window_handle != nullptr, "Input::Init called with null window_handle*");

		// setting input state for all keys/buttons to InputState::None
		for (int key = 0; key <= GLFW_KEY_LAST; key++)
			m_sKeyCache[static_cast<KeyCode>(key)] = InputState::None;

		for (int button = 0; button <= GLFW_MOUSE_BUTTON_LAST; button++)
			m_sMouseButtonCache[static_cast<MouseCode>(button)] = InputState::None;

		glfwSetKeyCallback(static_cast<GLFWwindow*>(window_handle), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == InputStateToGLFWBase(InputState::Pressed)) {
				m_sKeyCache[static_cast<KeyCode>(key)] = InputState::Pressed;
				//LOG(std::format("Key[{}] Pressed!", GetKeyCodeString(static_cast<KeyCode>(key))));
			}

			if (action == InputStateToGLFWBase(InputState::Released)) {
				m_sKeyCache[static_cast<KeyCode>(key)] = InputState::Released;
				//LOG(std::format("Key[{}] Released!", GetKeyCodeString(static_cast<KeyCode>(key))));
			}
	    });

		glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(window_handle), [](GLFWwindow* window, int button, int action, int mods) {
			if (action == InputStateToGLFWBase(InputState::Pressed)) {
				m_sMouseButtonCache[static_cast<MouseCode>(button)] = InputState::Pressed;
				//LOG(std::format("MouseButton[{}] Pressed!", GetMouseCodeString(static_cast<MouseCode>(button))));
			}

			if (action == InputStateToGLFWBase(InputState::Released)) {
				m_sMouseButtonCache[static_cast<MouseCode>(button)] = InputState::Released;
				//LOG(std::format("MouseButton[{}] Released!", GetMouseCodeString(static_cast<MouseCode>(button))));
			}
	    });

		glfwSetCursorPosCallback(static_cast<GLFWwindow*>(window_handle), [](GLFWwindow* window, double xpos, double ypos) {
			m_sMousePosition.x = xpos;
			m_sMousePosition.y = ypos;

			//LOG(std::format("CursorPositon(X, Y) : {}, {}", m_sMousePosition.x, m_sMousePosition.y));
	    });
	}

	void InputManager::Update() {
		m_sDeltaMousePosition = m_sMousePosition - m_sLastMousePosition;
		m_sLastMousePosition = m_sMousePosition;

		for (auto& [keycode, inputstate] : m_sKeyCache)
			m_sLastKeyCache[keycode] = inputstate;

		for (auto& [mousecode, inputstate] : m_sMouseButtonCache)
			m_sLastMouseButtonCache[mousecode] = inputstate;
	}

	// <--------------------- KEYBOARD ---------------------->

	bool InputManager::KeyDown(KeyCode key_code) {
		return m_sKeyCache[key_code] == InputState::Pressed && m_sLastKeyCache[key_code] != InputState::Pressed;
	}

	bool InputManager::KeyPressed(KeyCode key_code) {
		return m_sKeyCache[key_code] == InputState::Pressed;
	}

	bool InputManager::KeyReleased(KeyCode key_code) {
		return m_sKeyCache[key_code] == InputState::Released;
	}

	// <--------------------- MOUSE ---------------------->

	bool InputManager::MouseDown(MouseCode mouse_code) {
		return m_sMouseButtonCache[mouse_code] == InputState::Pressed && m_sLastMouseButtonCache[mouse_code] != InputState::Pressed;
	}

	bool InputManager::MousePressed(MouseCode mouse_code) {
		return m_sMouseButtonCache[mouse_code] == InputState::Released;
	}

	bool InputManager::MouseReleased(MouseCode mouse_code) {
		return m_sMouseButtonCache[mouse_code] == InputState::Released;
	}
}
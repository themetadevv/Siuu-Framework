
#include "afpch.h"
#include "assertion.h"

#include "application/logger.h"
#include "application/frames_manager.h"
#include "window.h"

std::string_view GetVideoModeString(VideoMode vm) {
	switch (vm) {
		case VideoMode::Default: {
			return "Default";
			break;
		}

		case VideoMode::Windowed: {
			return "Windowed";
			break;
		}

		case VideoMode::Borderless: {
			return "Borderless";
			break;
		}

		case VideoMode::Fullscreen: {
			return "Fullscreen";
			break;
		}
	}
}

namespace af {
	static bool s_Initialized = false;

	void Window::SaveWindowedSizePos() {
		glfwGetWindowPos(m_WindowHandle, &m_SavedWindowedPosition.x, &m_SavedWindowedPosition.y);
		glfwGetWindowSize(m_WindowHandle, &m_SavedWindowSize.x, &m_SavedWindowSize.y);
	}

	Window::Window(const WindowSpecs& window_specs) : m_WindowSpecs(window_specs), m_WindowHandle(nullptr), m_PrimaryMonitorHandle(nullptr), m_Running(true)
	{
		m_OldVideoMode = m_WindowSpecs.VidMode;
		m_SavedWindowSize = { 0, 0 };
		m_SavedWindowedPosition = { 0, 0 };

		int glfw_init_status = glfwInit();
		AF_ASSERT(glfw_init_status != 0, "glfwInit() returned 0!");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

		m_WindowHandle = glfwCreateWindow(m_WindowSpecs.Size.x, m_WindowSpecs.Size.y, m_WindowSpecs.Title.c_str(), nullptr, nullptr);
		AF_ASSERT(m_WindowHandle != nullptr, "GLFWwindow* is null, glfwCreateWindow might have failed!");

		glfwMakeContextCurrent(m_WindowHandle);
		glfwSwapInterval(m_WindowSpecs.VSync ? 1 : 0);
		glfwSetWindowUserPointer(m_WindowHandle, &m_WindowSpecs);

		m_PrimaryMonitorHandle = glfwGetPrimaryMonitor();
		AF_ASSERT(m_PrimaryMonitorHandle != nullptr, "GLFWmonitor* (m_PrimaryMonitorHandle) is null, glfwGetPrimaryMonitor might have failed!");

		// setting and logging some useful window callbacks

		glfwSetWindowPosCallback(m_WindowHandle, [](GLFWwindow* window, int xpos, int ypos) {
			WindowSpecs& window_data = *(WindowSpecs*)glfwGetWindowUserPointer(window);
			window_data.Position = { xpos, ypos };
	    });

		glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height) {
			WindowSpecs& window_data = *(WindowSpecs*)glfwGetWindowUserPointer(window);
			window_data.Size = { width, height };
	    });

		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* window) {
			AF_INFO("Program Elapsed Time : {0}", FrameManager::GetElapsedTime());
	    });

		glfwSetErrorCallback([](int error_code, const char* description) {
			AF_INFO("GLFW Error ({0}): {1}", error_code, description);
	    });

		SaveWindowedSizePos();

		if (m_WindowSpecs.VidMode != VideoMode::Windowed)
			SetVideoMode(m_WindowSpecs.VidMode);

		if (m_WindowSpecs.State != WindowState::None)
			SetWindowState(m_WindowSpecs.State);

		s_Initialized = true;
	}

	Window::~Window() {
		glfwDestroyWindow(m_WindowHandle);
		glfwTerminate();
	}

	void Window::SetWindowSize(const TVec2<int>& size) {
		m_WindowSpecs.Size = size;
		glfwSetWindowSize(m_WindowHandle, m_WindowSpecs.Size.x, m_WindowSpecs.Size.y);
	}

	void Window::SetWindowPosition(const TVec2<int>& pos) {
		m_WindowSpecs.Position = pos;
		glfwSetWindowPos(m_WindowHandle, m_WindowSpecs.Position.x, m_WindowSpecs.Position.y);
	}

	void Window::SetVSync(bool val) {
		m_WindowSpecs.VSync = val;
		glfwSwapInterval(m_WindowSpecs.VSync ? 1 : 0);
	}

	void Window::SetVideoMode(VideoMode vm) {
		if (m_WindowSpecs.VidMode == vm && s_Initialized)
			return;

		const GLFWvidmode* current_vid_mode = glfwGetVideoMode(m_PrimaryMonitorHandle);

		m_OldVideoMode = m_WindowSpecs.VidMode;
	//	AF_INFO("Old Video Mode : {0}", GetVideoModeString(m_OldVideoMode));

		switch (vm) {
			case VideoMode::Default: break;
			case VideoMode::Windowed: {
				glfwSetWindowAttrib(m_WindowHandle, GLFW_DECORATED, GLFW_TRUE);
				glfwSetWindowAttrib(m_WindowHandle, GLFW_RESIZABLE, GLFW_TRUE);

				glfwSetWindowMonitor(
					m_WindowHandle, nullptr,
					m_SavedWindowedPosition.x, m_SavedWindowedPosition.y,
					m_SavedWindowSize.x, m_SavedWindowSize.y, 0
				);

				m_WindowSpecs.VidMode = vm;
				break;
			}

			case VideoMode::Borderless: {
				glfwSetWindowAttrib(m_WindowHandle, GLFW_DECORATED, GLFW_FALSE);
				glfwSetWindowAttrib(m_WindowHandle, GLFW_RESIZABLE, GLFW_FALSE);

				if (m_OldVideoMode == VideoMode::Windowed)
					SaveWindowedSizePos();

				glfwSetWindowMonitor(
					m_WindowHandle, nullptr,
					0, 0,
					current_vid_mode->width, current_vid_mode->height, 0
				);

				m_WindowSpecs.VidMode = vm;
				break;
			}

			case VideoMode::Fullscreen: {
				glfwSetWindowAttrib(m_WindowHandle, GLFW_DECORATED, GLFW_FALSE);
				glfwSetWindowAttrib(m_WindowHandle, GLFW_RESIZABLE, GLFW_FALSE);

				if (m_OldVideoMode == VideoMode::Windowed)
					SaveWindowedSizePos();

				glfwSetWindowMonitor(
					m_WindowHandle, m_PrimaryMonitorHandle,
					0, 0, current_vid_mode->width, current_vid_mode->height,
					current_vid_mode->refreshRate
				);

				m_WindowSpecs.VidMode = vm;
				break;
			}
		}
	}

	void Window::SetWindowState(WindowState ws) {
		if (m_WindowSpecs.State == ws && s_Initialized)
			return;

		switch (ws) {
			case WindowState::None: break;

			case WindowState::Focused: {
				glfwRestoreWindow(m_WindowHandle);
				glfwShowWindow(m_WindowHandle);
				glfwFocusWindow(m_WindowHandle);
				break;
			}
			
			case WindowState::Maximized: {
				glfwMaximizeWindow(m_WindowHandle);
				break;
			}

			case WindowState::Minimized: {
				glfwIconifyWindow(m_WindowHandle);
				break;
			}
		}

		m_WindowSpecs.State = ws;
	}

	void Window::Update() {
		m_Running = !glfwWindowShouldClose(m_WindowHandle);

		glfwSwapBuffers(m_WindowHandle);
		glfwPollEvents();
	}

	void Window::Shutdown() {
		glfwSetWindowShouldClose(m_WindowHandle, true);
		m_Running = false;
	}

}
#pragma once

#include "core.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "maths.h"

enum class VideoMode {
	Default = 0,
	Windowed,
	Borderless,
	Fullscreen
};

AF_API std::string_view GetVideoModeString(VideoMode vm);

enum class WindowState : int {
	None = 0,
	Minimized,
	Maximized,
	Focused
};

struct WindowSpecs {
	std::string Title;
	TVec2<int> Size;
	TVec2<int> Position;
	VideoMode VidMode;
	WindowState State;
	bool VSync;

	WindowSpecs() {
		Title = "OpenGL : 4.6";
		Size = { 1600, 900 };
		Position = { 0, 0 };
		VidMode = VideoMode::Default;
		State = WindowState::None;
		VSync = false;
	}
};

namespace af {
	class AF_API Window {
	private:
		WindowSpecs m_WindowSpecs;
		GLFWwindow* m_WindowHandle;
		GLFWmonitor* m_PrimaryMonitorHandle;

		VideoMode  m_OldVideoMode;
		TVec2<int> m_SavedWindowSize;
		TVec2<int> m_SavedWindowedPosition;

		bool m_Running;

	private:
		void SaveWindowedSizePos();

	public:
		Window(const WindowSpecs& window_specs = WindowSpecs());
		~Window();

	public:
		// <------------------ Setters ------------------>

		void SetWindowSize(const TVec2<int>& size);
		void SetWindowPosition(const TVec2<int>& pos);
		void SetVSync(bool val);

		void SetVideoMode(VideoMode vm);
		void SetWindowState(WindowState ws);

	public:
		// <------------------ Getters ------------------>

		std::string_view GetWindowTitle() const { return m_WindowSpecs.Title; }

		TVec2<int> GetWindowSize() const { return m_WindowSpecs.Size; }
		TVec2<int> GetWindowPosition() const { return m_WindowSpecs.Position; }

		VideoMode GetCurrentVideoMode() const { return m_WindowSpecs.VidMode; }
		WindowState GetCurrentWindowState() const { return m_WindowSpecs.State; }

		GLFWwindow* GetNativeWindowHandle() const { return m_WindowHandle; }

		bool IsVSync() { return m_WindowSpecs.VSync; }

		

		bool IsRunning() { return m_Running; }

	public:
		// <------------------ Functions ------------------>

		void Update();
		void Shutdown();
	};
}


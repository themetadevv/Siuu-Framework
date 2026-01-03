
#include "afpch.h"
#include "assertion.h"
#include "frames_manager.h"


namespace af {
	double FrameManager::m_sStartFrameTime = 0.0;
	double FrameManager::m_sDeltaTime = 0.0;
	double FrameManager::m_sElapsedTime = 0.0;
	double FrameManager::m_sTargetFramesTime = 0.0;
	double FrameManager::m_sFrameTimeAccumulator = 0.0;

	uint32_t FrameManager::m_sTargetFrames = 0;
	uint32_t FrameManager::m_sFrameCount = 0;
	uint32_t FrameManager::m_sTotalFrames = 0;

	Window* FrameManager::m_Window = nullptr;

	void FrameManager::Init(Window* window) {
		m_Window = window;
		m_sStartFrameTime = glfwGetTime();
		AF_ASSERT(m_sStartFrameTime != 0.0, "glfwGetTime failed! did you create GLFWwindow context first?");
	}

	void FrameManager::SetTargetFrames(uint32_t frames) {
		m_sTargetFrames = frames;
		m_sTargetFramesTime = 1.0 / static_cast<double>(m_sTargetFrames);
		std::cout << "Target frame time: " << m_sTargetFramesTime << "s\n";
	}

	void FrameManager::Update() {
		double current_time = glfwGetTime();
		m_sDeltaTime = current_time - m_sStartFrameTime;

		// limmiting fps only if vsync is false (this limmiter sucks not precise)
		if (m_sTargetFrames > 0 && m_sDeltaTime < m_sTargetFramesTime && !m_Window->IsVSync()) {
			double sleep_duration = m_sTargetFramesTime - m_sDeltaTime;	
			std::this_thread::sleep_for(std::chrono::duration<double>(sleep_duration));

			// updating the delta_time after sleeping
			current_time = glfwGetTime();
			m_sDeltaTime = current_time - m_sStartFrameTime;
		}

		m_sStartFrameTime = current_time;
		m_sElapsedTime += m_sDeltaTime;

		m_sFrameCount++;
		m_sFrameTimeAccumulator += m_sDeltaTime;

		if (m_sFrameTimeAccumulator >= 1.0) {
			m_sTotalFrames = m_sFrameCount;
			m_sFrameCount = 0;
			m_sFrameTimeAccumulator -= 1.0;
		}
	}
}

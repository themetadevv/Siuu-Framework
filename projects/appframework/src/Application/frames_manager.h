#pragma once

#include "core.h"
#include "Maths.h"
#include "window/window.h"

// basic frame manager

namespace af {
	class AF_API FrameManager {
	private:
		static double m_sStartFrameTime;
		static double m_sElapsedTime;
		static double m_sDeltaTime;
		static double m_sTargetFramesTime;
		static double m_sFrameTimeAccumulator;

		static uint32_t m_sTargetFrames;
		static uint32_t m_sFrameCount;
		static uint32_t m_sTotalFrames;

		static Window* m_Window;

	public:
		static void Init(Window* window);
		static void SetTargetFrames(uint32_t frames);
		static void Update();

		static double GetDeltaTime() { return m_sDeltaTime; }
		static double GetElapsedTime() { return m_sElapsedTime; }
		static uint32_t GetTotalFrames() { return m_sTotalFrames; }
	};
}
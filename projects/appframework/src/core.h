#pragma once

#if defined(_MSC_VER)
	#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
	#define DEBUG_BREAK() __builtin_trap()
#else
#include <cstdlib>
	#define DEBUG_BREAK() std::abort()
#endif

#define ENABLE_LOGGING

#if defined(__DEBUG__BUILD__)
	#define ENABLE_ASSERTION

	#define SOUND_LIB_DEBUGGER
#endif

#if defined(BUILD_DLL)
	#define AF_API __declspec(dllexport)
#else
	#define AF_API __declspec(dllimport)
#endif

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

#define BIT(X) (1 << X)

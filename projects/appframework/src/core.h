#pragma once

#ifdef __DEBUG__BUILD__
	#define ENABLE_ASSERTION
	#define ENABLE_LOGGING
#endif

#ifdef __RELEASE__BUILD__
#define ENABLE_LOGGING
#endif

#ifdef BUILD_DLL
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

#ifdef ENABLE_ASSERTION

#include <iostream>
#include "utils.h"

inline bool GetAssertionMessage(bool condition, std::string_view message, const char* cond_name, const char* file, int line) {
	if (condition)
		return true;

	std::cout << "\n<--- " << "AppFramework->Assert" << " ----------------------------->\n";

	std::cout << "| Condition : ";
	std::cout << cond_name << '\n';

	std::cout << "| Location : ";
	std::cout << util::get_file_name_from_path(file) << "::" << line << '\n';

	std::cout << "| Message   : ";
	std::cout << message << '\n';

	std::cout << "<--------------------------------------------------->\n";

	return false;
}

#define AF_ASSERT(X, ...) \
	do { \
		if(!GetAssertionMessage(X, __VA_ARGS__, #X, __FILE__, __LINE__)) { \
			__debugbreak(); \
		} \
	} while (0)

#else
#define AF_ASSERT(X, ...) X;
#endif

#ifdef ENABLE_LOGGING
#define LOG(msg) \
	do { \
		std::cout << "[LOG]" << "[" << __TIME__ << "] " << msg << '\n'; \
	} while(0)
#else
#define LOG(msg);
#endif
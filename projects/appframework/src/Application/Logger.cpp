
#include "afpch.h"
#include "logger.h"

namespace af {	
	std::vector<spdlog::sink_ptr> Logger::m_LogSinks = {};
	Ref<spdlog::logger> Logger::m_sCoreLogger = nullptr;
	std::unordered_map <std::string, Ref<spdlog::logger>> Logger::m_sClients = {};
	uint32_t Logger::m_sClientsCount = 0;

	void Logger::Init() {
		m_LogSinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
		m_LogSinks[0]->set_pattern("%^[%T] %n: %v%$");
	}

	bool Logger::CreateCoreLogger() {
		if (m_sCoreLogger != nullptr) {
			AF_WARN("Core logger already initialized. Duplicate initialization request ignored.");
			return false;
		}

		if (m_LogSinks.empty()) {
			AF_ERROR("Logger::CreateCoreLogger() failed, Did you call Logger::Init() ?");
			return false;
		}

		m_sCoreLogger = CreateRef<spdlog::logger>("AF_CORE", begin(m_LogSinks), end(m_LogSinks));
		spdlog::register_logger(m_sCoreLogger);
		m_sCoreLogger->set_level(spdlog::level::trace);
		m_sCoreLogger->flush_on(spdlog::level::trace);
		return true;
	}

	bool Logger::AddClient(std::string_view client_name) {
		if (m_sClients.contains(std::string(client_name))) {
			AF_WARN("Client logger '{}' already exists. Creation request ignored.", client_name);
			return false;
		}

		if (m_LogSinks.empty()) {
			AF_ERROR("Logger::CreateCoreLogger() failed, Did you call Logger::Init() ?");
			return false;
		}

		auto logger = CreateRef<spdlog::logger>(std::string(client_name), begin(m_LogSinks), end(m_LogSinks));

		spdlog::register_logger(logger);
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
		m_sClients.emplace(client_name, logger);
		m_sClientsCount++;

		return true;
	}

	bool Logger::DeleteClient(std::string_view client_name) {
		std::string name = std::string(client_name);
		if (!m_sClients.contains(name)) {
			AF_ERROR("Logger::DeleteClient failed: client logger '{}' does not exist!", client_name);
			return false;
		}
		
		spdlog::drop(name);
		m_sClients.erase(name);
		m_sClientsCount--;

		return true;
	}

}


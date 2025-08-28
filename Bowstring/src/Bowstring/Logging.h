#pragma once
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace bowstring::log {

class Logger {
public:
  // Get the global logger instance
  static std::shared_ptr<spdlog::logger> &get() {
    static std::shared_ptr<spdlog::logger> instance = create();
    return instance;
  }

  // Convenience logging functions
  template <typename... Args>
  static void info(const char *fmt, const Args &...args) {
    get()->info(fmt, args...);
  }

  template <typename... Args>
  static void warn(const char *fmt, const Args &...args) {
    get()->warn(fmt, args...);
  }

  template <typename... Args>
  static void error(const char *fmt, const Args &...args) {
    get()->error(fmt, args...);
  }

  template <typename... Args>
  static void debug(const char *fmt, const Args &...args) {
    get()->debug(fmt, args...);
  }

private:
  Logger() = delete;

  // Create and configure the spdlog logger
  static std::shared_ptr<spdlog::logger> create() {
    auto logger = spdlog::stdout_color_mt("bowstring");
    logger->set_level(spdlog::level::debug);
    logger->set_pattern("[%H:%M:%S] [%^%l%$] %v");
    return logger;
  }
};

} // namespace bowstring::log
#define BS_LOG_INFO(...) bowstring::log::Logger::info(__VA_ARGS__)
#define BS_LOG_WARN(...) bowstring::log::Logger::warn(__VA_ARGS__)
#define BS_LOG_ERROR(...) bowstring::log::Logger::error(__VA_ARGS__)
#define BS_LOG_DEBUG(...) bowstring::log::Logger::debug(__VA_ARGS__)

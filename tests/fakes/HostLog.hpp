/**
 * @file HostLog.hpp
 * @brief Pretty host-test logging (timestamp, level, metadata, ANSI color).
 *
 * Used by MockI2C when call logging is enabled. Quiet unless callers emit.
 * Colors only when stderr is a TTY and NO_COLOR is unset.
 */
#ifndef INTROSATLIB_TEST_HOST_LOG_HPP_
#define INTROSATLIB_TEST_HOST_LOG_HPP_

#include <cstdarg>
#include <cstdint>
#include <string>

namespace IntroSatLib {
namespace test {

enum class LogLevel : uint8_t {
	Debug = 0,
	Info = 1,
	Warn = 2,
	Error = 3,
};

/**
 * Host log facade. Minimum level defaults to Debug; override with
 * INTROSATLIB_LOG_LEVEL=DEBUG|INFO|WARN|ERROR (case-insensitive).
 */
class HostLog {
public:
	static void setMinLevel(LogLevel level);
	static LogLevel minLevel();

	static void debug(const char* tag, const char* fmt, ...);
	static void info(const char* tag, const char* fmt, ...);
	static void warn(const char* tag, const char* fmt, ...);
	static void error(const char* tag, const char* fmt, ...);

	static void log(LogLevel level, const char* tag, const char* fmt, ...);
	static void vlog(LogLevel level, const char* tag, const char* fmt, va_list args);

	/** True when ANSI colors will be written. */
	static bool colorEnabled();
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_HOST_LOG_HPP_ */

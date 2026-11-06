/**
 * @file HostLogger.cpp
 * @brief Host LOGGER sink + HostLog pretty formatting for unit tests.
 *
 * - LOGGER(): raw/fragment writes to stderr (line-buffered with prefix once
 *   per completed line if a level was set via HostLog — otherwise plain).
 * - HostLog::*: timestamp + level + [tag] + message, optional ANSI colors.
 */
#include "HostLog.hpp"
#include "Logger.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mutex>
#include <string>

#if defined(_WIN32)
#include <io.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif

namespace IntroSatLib {
namespace test {
namespace {

std::mutex& logMutex()
{
	static std::mutex mutex;
	return mutex;
}

LogLevel& minLevelRef()
{
	static LogLevel level = [] {
		const char* env = std::getenv("INTROSATLIB_LOG_LEVEL");
		if (env == nullptr || env[0] == '\0') {
			return LogLevel::Debug;
		}
		if (std::strcmp(env, "INFO") == 0 || std::strcmp(env, "info") == 0) {
			return LogLevel::Info;
		}
		if (std::strcmp(env, "WARN") == 0 || std::strcmp(env, "warn") == 0
			|| std::strcmp(env, "WARNING") == 0 || std::strcmp(env, "warning") == 0) {
			return LogLevel::Warn;
		}
		if (std::strcmp(env, "ERROR") == 0 || std::strcmp(env, "error") == 0) {
			return LogLevel::Error;
		}
		return LogLevel::Debug;
	}();
	return level;
}

bool useColor()
{
	static const bool enabled = [] {
		// Explicit override wins over NO_COLOR / TTY detection.
		if (const char* v = std::getenv("INTROSATLIB_LOG_COLOR")) {
			if (v[0] == '0' && v[1] == '\0') {
				return false;
			}
			if (std::strcmp(v, "always") == 0 || std::strcmp(v, "1") == 0
				|| std::strcmp(v, "on") == 0) {
				return true;
			}
			if (std::strcmp(v, "never") == 0 || std::strcmp(v, "off") == 0) {
				return false;
			}
		}
		if (std::getenv("NO_COLOR") != nullptr) {
			return false;
		}
		return ISATTY(FILENO(stderr)) != 0;
	}();
	return enabled;
}

const char* levelName(LogLevel level)
{
	switch (level) {
	case LogLevel::Debug:
		return "DEBUG";
	case LogLevel::Info:
		return "INFO";
	case LogLevel::Warn:
		return "WARN";
	case LogLevel::Error:
		return "ERROR";
	}
	return "?";
}

const char* levelColor(LogLevel level)
{
	switch (level) {
	case LogLevel::Debug:
		return "\033[90m"; // bright black / gray
	case LogLevel::Info:
		return "\033[36m"; // cyan
	case LogLevel::Warn:
		return "\033[33m"; // yellow
	case LogLevel::Error:
		return "\033[31m"; // red
	}
	return "\033[0m";
}

constexpr const char* kReset = "\033[0m";
constexpr const char* kDim = "\033[2m";
constexpr const char* kBold = "\033[1m";

std::string formatTimestamp()
{
	using clock = std::chrono::system_clock;
	const auto now = clock::now();
	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	const std::time_t t = clock::to_time_t(now);
	std::tm tm{};
#if defined(_WIN32)
	localtime_s(&tm, &t);
#else
	localtime_r(&t, &tm);
#endif
	char buf[32];
	std::snprintf(
		buf,
		sizeof(buf),
		"%02d:%02d:%02d.%03d",
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		static_cast<int>(ms.count()));
	return buf;
}

void writeLine(LogLevel level, const char* tag, const char* message)
{
	if (static_cast<uint8_t>(level) < static_cast<uint8_t>(minLevelRef())) {
		return;
	}
	const std::string ts = formatTimestamp();
	const char* name = levelName(level);
	const bool color = useColor();

	std::lock_guard<std::mutex> lock(logMutex());
	if (color) {
		std::fprintf(
			stderr,
			"%s%s%s %s%s%-5s%s %s[%s]%s %s\n",
			kDim,
			ts.c_str(),
			kReset,
			levelColor(level),
			kBold,
			name,
			kReset,
			kDim,
			tag != nullptr ? tag : "-",
			kReset,
			message != nullptr ? message : "");
	} else {
		std::fprintf(
			stderr,
			"%s %-5s [%s] %s\n",
			ts.c_str(),
			name,
			tag != nullptr ? tag : "-",
			message != nullptr ? message : "");
	}
	std::fflush(stderr);
}

} /* namespace */

void HostLog::setMinLevel(LogLevel level) { minLevelRef() = level; }

LogLevel HostLog::minLevel() { return minLevelRef(); }

bool HostLog::colorEnabled() { return useColor(); }

void HostLog::debug(const char* tag, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog(LogLevel::Debug, tag, fmt, args);
	va_end(args);
}

void HostLog::info(const char* tag, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog(LogLevel::Info, tag, fmt, args);
	va_end(args);
}

void HostLog::warn(const char* tag, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog(LogLevel::Warn, tag, fmt, args);
	va_end(args);
}

void HostLog::error(const char* tag, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog(LogLevel::Error, tag, fmt, args);
	va_end(args);
}

void HostLog::log(LogLevel level, const char* tag, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog(level, tag, fmt, args);
	va_end(args);
}

void HostLog::vlog(LogLevel level, const char* tag, const char* fmt, va_list args)
{
	char body[1024];
	if (fmt == nullptr) {
		body[0] = '\0';
	} else {
		std::vsnprintf(body, sizeof(body), fmt, args);
	}
	writeLine(level, tag, body);
}

} /* namespace test */
} /* namespace IntroSatLib */

namespace {

/** Fragments from logText/logHEX until a newline — then emit as HostLog INFO. */
std::string& loggerLineBuffer()
{
	static std::string buffer;
	return buffer;
}

} /* namespace */

void LOGGER(const char* text, uint8_t len)
{
	if (text == nullptr || len == 0) {
		return;
	}
	std::string& buffer = loggerLineBuffer();
	buffer.append(text, len);
	for (;;) {
		const auto pos = buffer.find('\n');
		if (pos == std::string::npos) {
			break;
		}
		std::string line = buffer.substr(0, pos);
		buffer.erase(0, pos + 1);
		IntroSatLib::test::HostLog::info("Logger", "%s", line.c_str());
	}
}

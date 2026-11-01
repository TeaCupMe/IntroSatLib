#ifndef HEADLESS_LOG_HEADLESSLOG_H_
#define HEADLESS_LOG_HEADLESSLOG_H_

/**
 * Tiny host-side logging sketch for headless demos.
 * See docs/headless-logging.md — default Off (no spam).
 * Not a replacement for src/Logger.h; same spirit, stderr sink.
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace IntroSatLib {
namespace headless {

enum class LogLevel : int { Off = 0, Error = 1, Info = 2, Debug = 3 };

inline LogLevel& logLevelRef() {
	static LogLevel level = LogLevel::Off;
	return level;
}

inline const char*& logProcRef() {
	static const char* proc = "headless";
	return proc;
}

inline LogLevel parseLogLevel(const char* s) {
	if (s == nullptr || s[0] == '\0') {
		return LogLevel::Off;
	}
	if (std::strcmp(s, "off") == 0) {
		return LogLevel::Off;
	}
	if (std::strcmp(s, "error") == 0) {
		return LogLevel::Error;
	}
	if (std::strcmp(s, "info") == 0) {
		return LogLevel::Info;
	}
	if (std::strcmp(s, "debug") == 0) {
		return LogLevel::Debug;
	}
	return LogLevel::Off;
}

/** CLI overrides env; call once from main after parsing argv. */
inline void logInit(const char* proc, LogLevel cliLevel, bool cliSet) {
	logProcRef() = proc != nullptr ? proc : "headless";
	if (cliSet) {
		logLevelRef() = cliLevel;
		return;
	}
	if (const char* env = std::getenv("INTROSATLIB_HEADLESS_LOG")) {
		logLevelRef() = parseLogLevel(env);
	} else {
		logLevelRef() = LogLevel::Off;
	}
}

inline const char* levelName(LogLevel level) {
	switch (level) {
	case LogLevel::Error:
		return "ERROR";
	case LogLevel::Info:
		return "INFO";
	case LogLevel::Debug:
		return "DEBUG";
	default:
		return "OFF";
	}
}

inline void logMsg(LogLevel level, const char* event, const std::string& kv = {}) {
	if (static_cast<int>(level) > static_cast<int>(logLevelRef()) ||
	    level == LogLevel::Off) {
		return;
	}
	std::cerr << "isl[" << logProcRef() << "] " << levelName(level) << "  "
		  << event;
	if (!kv.empty()) {
		std::cerr << " " << kv;
	}
	std::cerr << "\n";
}

} /* namespace headless */
} /* namespace IntroSatLib */

#endif /* HEADLESS_LOG_HEADLESSLOG_H_ */

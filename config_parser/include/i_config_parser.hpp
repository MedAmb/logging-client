#ifndef LOGGING_CONFIG_PARSER_I_CONFIG_PARSER_HPP
#define LOGGING_CONFIG_PARSER_I_CONFIG_PARSER_HPP

#include <array>
#include <chrono>
#include <map>
#include <string_view>

namespace logger {
namespace config_parser {

struct LogLimits {
  std::chrono::milliseconds limitPeriodMs;
  std::size_t logLimitDuringPeriodInBytes;
};

inline bool operator==(const LogLimits &lhs, const LogLimits &rhs) {
  return lhs.limitPeriodMs == rhs.limitPeriodMs &&
         lhs.logLimitDuringPeriodInBytes == rhs.logLimitDuringPeriodInBytes;
}

class IConfigParser {
 public:
  IConfigParser() = default;
  IConfigParser(const IConfigParser &) = delete;
  IConfigParser(IConfigParser &&) = delete;
  IConfigParser &operator=(const IConfigParser &) = delete;
  IConfigParser &operator=(IConfigParser &&) = delete;

  virtual ~IConfigParser() = default;

  virtual const LogLimits &GetLogLimits() const = 0;

  virtual const std::array<char, 4> &GetDefaultLogContext() const = 0;

  virtual const std::chrono::milliseconds &GetFlushPeriod() const = 0;

  virtual const std::string_view &GetTriggerFlushIfLogLevelReaches() const = 0;

  virtual const std::string_view &GetLogAppenderType() const = 0;

  virtual const std::map<std::string_view, std::string_view>
      &GetLogAppenderParams() const = 0;
};

}  // namespace config_parser
}  // namespace logger

#endif  // LOGGING_CONFIG_PARSER_I_CONFIG_PARSER_HPP

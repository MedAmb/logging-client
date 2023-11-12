#ifndef LOGGING_CONFIG_PARSER_I_CONFIG_PARSER_HPP
#define LOGGING_CONFIG_PARSER_I_CONFIG_PARSER_HPP

#include <chrono>
#include <map>
#include <string>

namespace logger {
namespace config_parser {

struct LogLimits {
  std::chrono::milliseconds period;
  std::size_t maxSizeDuringPeriod;
};

inline bool operator==(const LogLimits &lhs, const LogLimits &rhs) {
  return lhs.period == rhs.period &&
         lhs.maxSizeDuringPeriod == rhs.maxSizeDuringPeriod;
}

class IConfigParser {
 public:
  IConfigParser() = default;
  IConfigParser(const IConfigParser &) = delete;
  IConfigParser(IConfigParser &&) = delete;
  IConfigParser &operator=(const IConfigParser &) = delete;
  IConfigParser &operator=(IConfigParser &&) = delete;

  virtual ~IConfigParser() = default;

  virtual LogLimits GetLogLimits() const = 0;

  virtual std::string GetDefaultLogContextId() const = 0;

  virtual std::string GetLogAppenderType() const = 0;

  virtual const std::map<std::string, std::string> &GetLogAppenderParams()
      const = 0;
};

}  // namespace config_parser
}  // namespace logger

#endif  // LOGGING_CONFIG_PARSER_I_CONFIG_PARSER_HPP

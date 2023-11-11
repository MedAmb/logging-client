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

class IConfigParser {
public:
  IConfigParser() = default;
  IConfigParser(const IConfigParser &) = delete;
  IConfigParser(IConfigParser &&) = delete;
  IConfigParser &operator=(const IConfigParser &) = delete;
  IConfigParser &operator=(IConfigParser &&) = delete;

  virtual ~IConfigParser() = default;

  virtual LogLimits getLogLimits() const = 0;

  virtual std::string getDefaultLogContextId() const = 0;

  virtual std::string getLogAppenderType() const = 0;

  virtual const std::map<std::string, std::string> &
  getLogAppenderParams() const = 0;
};

} // namespace config_parser
} // namespace logger

#endif // LOGGING_CONFIG_PARSER_I_CONFIG_PARSER_HPP
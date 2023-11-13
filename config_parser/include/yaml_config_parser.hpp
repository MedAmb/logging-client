#ifndef LOGGING_CONFIG_PARSER_YAML_CONFIG_PARSER_HPP
#define LOGGING_CONFIG_PARSER_YAML_CONFIG_PARSER_HPP

#include "i_config_parser.hpp"
#include "yaml-cpp/yaml.h"

namespace logger {
namespace config_parser {

class YamlConfigParser : public IConfigParser {
 public:
  explicit YamlConfigParser(const std::string &cfg_file_path);

  YamlConfigParser(const YamlConfigParser &) = delete;
  YamlConfigParser(YamlConfigParser &&) = delete;
  YamlConfigParser &operator=(YamlConfigParser &&) = delete;
  YamlConfigParser &operator=(const YamlConfigParser &) = delete;

  ~YamlConfigParser() override = default;

  const LogLimits &GetLogLimits() const override;

  const std::array<char, 4> &GetDefaultLogContext() const override;

  const std::chrono::milliseconds &GetFlushPeriod() const override;

  const std::string_view &GetTriggerFlushIfLogLevelReaches() const override;

  const std::string_view &GetLogAppenderType() const override;

  const std::map<std::string_view, std::string_view> &GetLogAppenderParams()
      const override;

 private:
  LogLimits ParseLogLimits();
  std::array<char, 4> ParseDefaultLogContext();
  std::chrono::milliseconds ParseFlushPeriod();
  std::string_view ParseTriggerFlushIfLogLevelReaches();
  std::string_view ParseLogAppenderType();
  std::map<std::string_view, std::string_view> ParseLogAppenderParams();

 private:
  YAML::Node config_;
  const LogLimits log_limits_;
  const std::array<char, 4> default_log_context_;
  const std::chrono::milliseconds flush_period_ms_;
  const std::string_view trigger_flush_if_log_level_reaches_;
  const std::string_view log_appender_type_;
  const std::map<std::string_view, std::string_view> log_appender_params_;
};

}  // namespace config_parser
}  // namespace logger

#endif  // LOGGING_CONFIG_PARSER_YAML_CONFIG_PARSER_HPP

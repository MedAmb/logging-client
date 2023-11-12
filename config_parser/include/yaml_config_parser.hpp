#ifndef LOGGING_CONFIG_PARSER_YAML_CONFIG_PARSER_HPP
#define LOGGING_CONFIG_PARSER_YAML_CONFIG_PARSER_HPP

#include <memory>

#include "i_config_parser.hpp"
#include "yaml-cpp/yaml.h"

namespace logger {
namespace config_parser {

class YamlConfigParser : public IConfigParser {
 public:
  YamlConfigParser(const YamlConfigParser &) = delete;
  YamlConfigParser(YamlConfigParser &&) = delete;
  YamlConfigParser &operator=(YamlConfigParser &&) = delete;
  YamlConfigParser &operator=(const YamlConfigParser &) = delete;

  ~YamlConfigParser() override = default;

  LogLimits GetLogLimits() const override;

  std::string GetDefaultLogContextId() const override;

  std::string GetLogAppenderType() const override;

  const std::map<std::string, std::string> &GetLogAppenderParams()
      const override;

  // can throw YAML::BadFile, YAML::InvalidNode, YAML::BadConversion,
  static YamlConfigParser &Create(const std::string_view &cfg_file_path);

  static YamlConfigParser &Get();

 private:
  explicit YamlConfigParser(const std::string_view &cfg_file_path);

  LogLimits ParseLogLimits();
  std::string ParseDefaultLogContextId();
  std::string ParseLogAppenderType();
  std::map<std::string, std::string> ParseLogAppenderParams();

 private:
  static std::unique_ptr<YamlConfigParser> instance_;

  YAML::Node config_;
  const LogLimits log_limits_;
  const std::string default_log_context_id_;
  const std::string log_appender_type_;
  const std::map<std::string, std::string> log_appender_params_;
};

}  // namespace config_parser
}  // namespace logger

#endif  // LOGGING_CONFIG_PARSER_YAML_CONFIG_PARSER_HPP

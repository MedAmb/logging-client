#include "yaml_config_parser.hpp"

namespace logger {
namespace config_parser {

YamlConfigParser &YamlConfigParser::create(
    const std::string_view &cfg_file_path) {
  static YamlConfigParser instance(cfg_file_path);
  return instance;
}

YamlConfigParser::YamlConfigParser(const std::string_view &cfg_file_path)
    : config_{YAML::LoadFile(cfg_file_path.data())},
      log_limits_{ParseLogLimits()},
      default_log_context_id_{ParseDefaultLogContextId()},
      log_appender_type_{ParseLogAppenderType()},
      log_appender_params_{ParseLogAppenderParams()} {}

LogLimits YamlConfigParser::getLogLimits() const { return log_limits_; }

std::string YamlConfigParser::getDefaultLogContextId() const {
  return default_log_context_id_;
}

std::string YamlConfigParser::getLogAppenderType() const {
  return log_appender_type_;
}

const std::map<std::string, std::string>
    &YamlConfigParser::getLogAppenderParams() const {
  return log_appender_params_;
}

LogLimits YamlConfigParser::ParseLogLimits() {
  const auto log_limits_node = config_["log_limits"];
  const auto period = std::chrono::milliseconds(
      log_limits_node["period_ms"].as<std::uint64_t>());
  const auto max_size_during_period =
      log_limits_node["log_size_limit_during_period_in_bytes"]
          .as<std::size_t>();
  return {period, max_size_during_period};
}

std::string YamlConfigParser::ParseDefaultLogContextId() {
  return config_["default_log_context_id"].as<std::string>();
}

std::string YamlConfigParser::ParseLogAppenderType() {
  return config_["log_appender"]["type"].as<std::string>();
}

std::map<std::string, std::string> YamlConfigParser::ParseLogAppenderParams() {
  std::map<std::string, std::string> log_appender_params;
  const auto log_appender_params_node = config_["log_appender"]["params"];
  for (const auto &param : log_appender_params_node) {
    log_appender_params[param.first.as<std::string>()] =
        param.second.as<std::string>();
  }
  return log_appender_params;
}

}  // namespace config_parser
}  // namespace logger

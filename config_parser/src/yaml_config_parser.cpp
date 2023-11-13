#include "yaml_config_parser.hpp"

#include <algorithm>

#include "config_constants.hpp"

namespace logger {
namespace config_parser {

YamlConfigParser::YamlConfigParser(const std::string &cfg_file_path)
    : config_{YAML::LoadFile(cfg_file_path)},
      log_limits_{ParseLogLimits()},
      default_log_context_{ParseDefaultLogContext()},
      flush_period_ms_{ParseFlushPeriod()},
      trigger_flush_if_log_level_reaches_{ParseTriggerFlushIfLogLevelReaches()},
      log_appender_type_{ParseLogAppenderType()},
      log_appender_params_{ParseLogAppenderParams()} {}

const LogLimits &YamlConfigParser::GetLogLimits() const { return log_limits_; }

const std::array<char, 4> &YamlConfigParser::GetDefaultLogContext() const {
  return default_log_context_;
}

const std::chrono::milliseconds &YamlConfigParser::GetFlushPeriod() const {
  return flush_period_ms_;
}

const std::string_view &YamlConfigParser::GetTriggerFlushIfLogLevelReaches()
    const {
  return trigger_flush_if_log_level_reaches_;
}

const std::string_view &YamlConfigParser::GetLogAppenderType() const {
  return log_appender_type_;
}

const std::map<std::string_view, std::string_view>
    &YamlConfigParser::GetLogAppenderParams() const {
  return log_appender_params_;
}

LogLimits YamlConfigParser::ParseLogLimits() {
  LogLimits log_limits;

  if (!config_[kLogLimitsKey]) {
    log_limits = {std::chrono::milliseconds{kDefaultLimitPeriodMs},
                  kDefaultLogSizeLimitDuringPeriodInBytes};
  } else {
    log_limits = {
        std::chrono::milliseconds{
            config_[kLogLimitsKey][kLimitPeriodMsKey].as<std::uint64_t>()},
        config_[kLogLimitsKey][kLogSizeLimitDuringPeriodInBytesKey]
            .as<std::size_t>()};
  }

  return log_limits;
}

std::array<char, 4> YamlConfigParser::ParseDefaultLogContext() {
  std::array<char, 4> default_log_context;

  if (!config_[kDefaultLogContextKey]) {
    default_log_context = kDefaultLogContext;
  } else {
    std::string_view default_log_context_str =
        config_[kDefaultLogContextKey].as<std::string_view>();
    std::copy_n(
        default_log_context_str.begin(),
        std::min(default_log_context_str.size(), default_log_context.size()),
        default_log_context.begin());
  }
  return default_log_context;
}

std::chrono::milliseconds YamlConfigParser::ParseFlushPeriod() {
  std::chrono::milliseconds flush_period_ms;

  if (!config_[kFlushPeriodMsKey]) {
    flush_period_ms = std::chrono::milliseconds{kDefaultFlushPeriodMs};
  } else {
    flush_period_ms = std::chrono::milliseconds{
        config_[kFlushPeriodMsKey].as<std::uint64_t>()};
  }
  return flush_period_ms;
}

std::string_view YamlConfigParser::ParseTriggerFlushIfLogLevelReaches() {
  std::string_view trigger_flush_if_log_level_reaches;

  if (!config_[kTriggerFlushIfLogLevelReaches]) {
    trigger_flush_if_log_level_reaches = kDefaultTriggerFlushIfLogLevelReaches;
  } else {
    trigger_flush_if_log_level_reaches =
        config_[kTriggerFlushIfLogLevelReaches].as<std::string_view>();
  }

  return trigger_flush_if_log_level_reaches;
}

std::string_view YamlConfigParser::ParseLogAppenderType() {
  std::string_view log_appender_type;

  if (!config_[kLogAppenderKey][kLogAppenderTypeKey]) {
    log_appender_type = kLogAppenderTypeDefault;
  } else {
    log_appender_type =
        config_[kLogAppenderKey][kLogAppenderTypeKey].as<std::string_view>();
  }
  return log_appender_type;
}

std::map<std::string_view, std::string_view>
YamlConfigParser::ParseLogAppenderParams() {
  std::map<std::string_view, std::string_view> log_appender_params;

  if (!config_[kLogAppenderKey][kLogAppenderParamsKey]) {
    log_appender_params = {};
  } else {
    log_appender_params =
        config_[kLogAppenderKey][kLogAppenderParamsKey]
            .as<std::map<std::string_view, std::string_view>>();
  }

  return log_appender_params;
}

}  // namespace config_parser
}  // namespace logger

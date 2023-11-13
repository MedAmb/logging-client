#include <array>
#include <string_view>

namespace logger {
namespace config_parser {

constexpr std::string_view kLogLimitsKey{"log_limits"};

constexpr std::string_view kLimitPeriodMsKey{"limit_period_in_ms"};
constexpr std::uint64_t kDefaultLimitPeriodMs{1000};

constexpr std::string_view kLogSizeLimitDuringPeriodInBytesKey{
    "log_size_limit_during_period_in_bytes"};
constexpr std::size_t kDefaultLogSizeLimitDuringPeriodInBytes{1000};

constexpr std::string_view kDefaultLogContextKey{"default_log_context"};
constexpr std::array<char, 4> kDefaultLogContext{'D', 'F', 'L', 'T'};

constexpr std::string_view kFlushPeriodMsKey{"flush_period_in_ms"};
constexpr std::uint64_t kDefaultFlushPeriodMs{1000};

constexpr std::string_view kTriggerFlushIfLogLevelReaches{
    "trigger_flush_if_log_level_reaches"};
constexpr std::string_view kDefaultTriggerFlushIfLogLevelReaches{"CRITICAL"};

constexpr std::string_view kLogAppenderKey{"log_appender"};
constexpr std::string_view kLogAppenderTypeKey{"type"};
constexpr std::string_view kLogAppenderTypeDefault{"console"};
constexpr std::string_view kLogAppenderParamsKey{"params"};

}  // namespace config_parser
}  // namespace logger

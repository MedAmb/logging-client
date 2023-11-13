#include "yaml_config_parser.hpp"

#include <filesystem>
#include <fstream>

#include "config_constants.hpp"
#include "gtest/gtest.h"

using namespace logger::config_parser;

class YamlConfigParserTestFixture : public ::testing::Test {
 public:
  YamlConfigParserTestFixture() = default;
  ~YamlConfigParserTestFixture() override = default;

  void TearDown() override { std::filesystem::remove(cfg_file_path_); }

  const std::string& GetConfigFilePath() const { return cfg_file_path_; }

  void CreateConfigFile(const std::string& content) {
    ofs_.open(cfg_file_path_);
    ofs_ << content;
    ofs_.close();
  }

 private:
  const std::string cfg_file_path_ = "/tmp/test.yaml";
  std::ofstream ofs_{};
};

TEST_F(YamlConfigParserTestFixture, YamlConfigParserFileDoesNotExist) {
  EXPECT_THROW(YamlConfigParser{GetConfigFilePath()}, YAML::BadFile);
}

TEST_F(YamlConfigParserTestFixture, YamlConfigParserFileIsNotYaml) {
  CreateConfigFile("some content\nsome content\n");
  EXPECT_THROW(YamlConfigParser{GetConfigFilePath()}, YAML::BadSubscript);
}

TEST_F(YamlConfigParserTestFixture, YamlConfigParserYamlIsMalformed) {
  CreateConfigFile("some content\nsome: content\nsome: content\n");
  EXPECT_THROW(YamlConfigParser{GetConfigFilePath()}, YAML::ParserException);
}

TEST_F(YamlConfigParserTestFixture,
       YamlConfigParserBadConversionlimit_period_in_ms) {
  CreateConfigFile(
      "log_limits:\n"
      "  limit_period_in_ms: 10k00\n"  // bad conversion
      "  log_size_limit_during_period_in_bytes: 1000\n");

  EXPECT_THROW(YamlConfigParser{GetConfigFilePath()}, YAML::BadConversion);
}

TEST_F(YamlConfigParserTestFixture,
       YamlConfigParserBadConversionlog_size_limit_during_period_in_bytes) {
  CreateConfigFile(
      "log_limits:\n"
      "  limit_period_in_ms: 1000\n"
      "  log_size_limit_during_period_in_bytes: 10k00\n");  // bad conversion

  EXPECT_THROW(YamlConfigParser{GetConfigFilePath()}, YAML::BadConversion);
}

TEST_F(YamlConfigParserTestFixture,
       YamlConfigParserBadConversionflush_period_in_ms) {
  CreateConfigFile("flush_period_in_ms: 10k00\n");  // bad conversion
  EXPECT_THROW(YamlConfigParser{GetConfigFilePath()}, YAML::BadConversion);
}

TEST_F(YamlConfigParserTestFixture,
       YamlConfigParserBadConversionIllformedParamMap) {
  CreateConfigFile(
      "log_appender:\n"
      "  type: \"console\"\n"
      "  params:\n"
      "     some:\n"
      "     some: content\n");

  EXPECT_THROW(YamlConfigParser{GetConfigFilePath()}, YAML::BadConversion);
}

TEST_F(YamlConfigParserTestFixture,
       YamlConfigParserUsingDefaultValuesWhenConfigIsEmpty) {
  CreateConfigFile("");
  auto config = YamlConfigParser{GetConfigFilePath()};

  EXPECT_EQ(config.GetLogLimits().limitPeriodMs,
            std::chrono::milliseconds{kDefaultLimitPeriodMs});
  EXPECT_EQ(config.GetLogLimits().logLimitDuringPeriodInBytes,
            kDefaultLogSizeLimitDuringPeriodInBytes);
  EXPECT_EQ(config.GetDefaultLogContext(), kDefaultLogContext);
  EXPECT_EQ(config.GetFlushPeriod(),
            std::chrono::milliseconds{kDefaultFlushPeriodMs});
  EXPECT_EQ(config.GetTriggerFlushIfLogLevelReaches(),
            kDefaultTriggerFlushIfLogLevelReaches);
  EXPECT_EQ(config.GetLogAppenderType(), kLogAppenderTypeDefault);
  EXPECT_TRUE(config.GetLogAppenderParams().empty());
}

TEST_F(YamlConfigParserTestFixture,
       YamlConfigParserUsingDefaultForMissingValue) {
  CreateConfigFile(
      "default_log_context: DFLT\n"
      "flush_period_in_ms: 1000\n"
      "trigger_flush_if_log_level_reaches: CRITICAL\n"
      "log_appender:\n"
      "  type: \"console\"\n"
      "  params:\n"
      "    format: \"[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%c] [%f:%n] [%v]\"\n");

  auto config = YamlConfigParser{GetConfigFilePath()};

  EXPECT_EQ(config.GetLogLimits().limitPeriodMs,
            std::chrono::milliseconds{kDefaultLimitPeriodMs});
  EXPECT_EQ(config.GetLogLimits().logLimitDuringPeriodInBytes,
            kDefaultLogSizeLimitDuringPeriodInBytes);

  auto default_log_context = std::array<char, 4>{'D', 'F', 'L', 'T'};
  EXPECT_EQ(config.GetDefaultLogContext(), default_log_context);

  EXPECT_EQ(config.GetFlushPeriod(), std::chrono::milliseconds{1000});

  EXPECT_EQ(config.GetTriggerFlushIfLogLevelReaches(), "CRITICAL");

  EXPECT_EQ(config.GetLogAppenderType(), "console");

  auto params = config.GetLogAppenderParams();

  EXPECT_EQ(params.at("format"),
            "[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%c] [%f:%n] [%v]");
}

TEST_F(YamlConfigParserTestFixture, YamlConfigParserUsingProvidedValues) {
  CreateConfigFile(
      "log_limits:\n"
      "  limit_period_in_ms: 1000\n"
      "  log_size_limit_during_period_in_bytes: 1000\n"
      "default_log_context: DFLT\n"
      "flush_period_in_ms: 1000\n"
      "trigger_flush_if_log_level_reaches: CRITICAL\n"
      "log_appender:\n"
      "  type: \"console\"\n"
      "  params:\n"
      "    format: \"[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%c] [%f:%n] [%v]\"\n");

  auto config = YamlConfigParser{GetConfigFilePath()};

  EXPECT_EQ(config.GetLogLimits().limitPeriodMs,
            std::chrono::milliseconds{1000});
  EXPECT_EQ(config.GetLogLimits().logLimitDuringPeriodInBytes, 1000);

  auto default_log_context = std::array<char, 4>{'D', 'F', 'L', 'T'};
  EXPECT_EQ(config.GetDefaultLogContext(), default_log_context);

  EXPECT_EQ(config.GetFlushPeriod(), std::chrono::milliseconds{1000});

  EXPECT_EQ(config.GetTriggerFlushIfLogLevelReaches(), "CRITICAL");

  EXPECT_EQ(config.GetLogAppenderType(), "console");

  auto params = config.GetLogAppenderParams();

  EXPECT_EQ(params.at("format"),
            "[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%c] [%f:%n] [%v]");
}

#include "yaml_config_parser.hpp"

#include <fstream>

#include "gtest/gtest.h"

TEST(YamlConfigParserTest, CreateFailFileDoesNotExist) {
  const std::string_view cfg_file_path = "/path/does/not/exist";
  EXPECT_THROW(logger::config_parser::YamlConfigParser::Create(cfg_file_path),
               YAML::BadFile);
}

TEST(YamlConfigParserTest, CreateFailFileIsNotYaml) {
  const std::string_view cfg_file_path = "/tmp/test.yaml";
  std::ofstream ofs(cfg_file_path.data());
  ofs << "some content";
  ofs.close();
  EXPECT_THROW(logger::config_parser::YamlConfigParser::Create(cfg_file_path),
               YAML::BadSubscript);
  std::remove(cfg_file_path.data());
}

TEST(YamlConfigParserTest, CreateFailFileIsNotValidYaml) {
  const std::string_view cfg_file_path = "/tmp/test.yaml";
  std::ofstream ofs(cfg_file_path.data());
  ofs << "some: content";
  ofs.close();
  EXPECT_THROW(logger::config_parser::YamlConfigParser::Create(cfg_file_path),
               YAML::InvalidNode);
  std::remove(cfg_file_path.data());
}

TEST(YamlConfigParserTest, CreateFailBadLogLimits) {
  const std::string_view cfg_file_path = "/tmp/test.yaml";
  std::ofstream ofs(cfg_file_path.data());
  ofs << "log_limits:\n"
         "  period_ms: period\n"
         "  log_size_limit_during_period_in_bytes: 1000\n"
         "default_log_context_id: \"default\"\n"
         "log_appender:\n"
         "  type: \"file\"\n"
         "  params:\n"
         "    file_path: \"/tmp/test.log\"\n";
  ofs.close();
  EXPECT_THROW(logger::config_parser::YamlConfigParser::Create(cfg_file_path),
               YAML::BadConversion);
  std::remove(cfg_file_path.data());
}

TEST(YamlConfigParserTest, WrongGetCall) {
  try {
    logger::config_parser::YamlConfigParser::Get();
    FAIL() << "Expected std::runtime_error";
  } catch (std::runtime_error const& err) {
    EXPECT_EQ(err.what(),
              std::string("YamlConfigParser has not been created yet"));
  } catch (...) {
    FAIL() << "Expected std::runtime_error";
  }
}

TEST(YamlConfigParserTest, YamlParsedCorrectly) {
  const std::string_view cfg_file_path = "/tmp/test.yaml";
  std::ofstream ofs(cfg_file_path.data());
  ofs << "log_limits:\n"
         "  period_ms: 1000\n"
         "  log_size_limit_during_period_in_bytes: 1000\n"
         "default_log_context_id: DFLT\n"
         "log_appender:\n"
         "  type: \"file\"\n"
         "  params:\n"
         "    file_path: \"/tmp/test.log\"\n"
         "    rotating: true\n"
         "    max_size_in_bytes: 1000\n"
         "    max_files: 10\n"
         "    format: \"[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%c] [%f:%n] [%v]\"\n";

  ofs.close();

  EXPECT_NO_THROW(
      logger::config_parser::YamlConfigParser::Create(cfg_file_path));
  auto& parser = logger::config_parser::YamlConfigParser::Get();

  auto log_limits =
      logger::config_parser::LogLimits{std::chrono::milliseconds{1000}, 1000};

  EXPECT_EQ(parser.GetLogLimits(), log_limits);
  EXPECT_EQ(parser.GetDefaultLogContextId(), "DFLT");
  EXPECT_EQ(parser.GetLogAppenderType(), "file");

  auto params = parser.GetLogAppenderParams();

  EXPECT_EQ(params.at("file_path"), "/tmp/test.log");
  EXPECT_EQ(params.at("rotating"), "true");
  EXPECT_EQ(params.at("max_size_in_bytes"), "1000");
  EXPECT_EQ(params.at("max_files"), "10");
  EXPECT_EQ(params.at("format"),
            "[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%c] [%f:%n] [%v]");

  std::remove(cfg_file_path.data());
}

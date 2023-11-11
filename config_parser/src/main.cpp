#include "yaml_config_parser.hpp"
#include <iostream>

int main() {

    const auto &config_parser =
        logger::config_parser::YamlConfigParser::create("/home/mohamed/workspace/logging/config.yaml");

    std::cout << "Log limits: " << std::endl;
    std::cout << "  period: " << config_parser.getLogLimits().period.count()
                << std::endl;
    std::cout << "  max_size_during_period: "
                << config_parser.getLogLimits().maxSizeDuringPeriod << std::endl;

    std::cout << "Default log context id: " << config_parser.getDefaultLogContextId()
                << std::endl;

    std::cout << "Log appender type: " << config_parser.getLogAppenderType()
                << std::endl;

    std::cout << "Log appender params: " << std::endl;
    for (const auto &[key, value] : config_parser.getLogAppenderParams()) {
        std::cout << "  " << key << ": " << value << std::endl;
    }


    return 0;

}
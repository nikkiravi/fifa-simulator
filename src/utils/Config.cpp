#include "utils/Config.h"
#include <iostream>

Config* Config::instance = nullptr;

Config& Config::getInstance(const std::string& filepath) {
    if (!instance) {
        instance = new Config(filepath);
    }
    return *instance;
}

Config::Config(const std::string& filepath) {
    loadEnvFile(filepath);
}

void Config::loadEnvFile(const std::string& filepath) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Find the '=' delimiter
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n\""));
            value.erase(value.find_last_not_of(" \t\r\n\"") + 1);

            settings[key] = value;
        }
    }

    std::cout << "Loaded " << settings.size() << " configuration settings from " << filepath << std::endl;
}

std::string Config::get(const std::string& key) const {
    auto it = settings.find(key);
    if (it != settings.end()) {
        return it->second;
    }
    throw std::runtime_error("Config key not found: " + key);
}

bool Config::has(const std::string& key) const {
    return settings.find(key) != settings.end();
}
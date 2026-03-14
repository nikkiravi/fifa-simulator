#pragma once
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>

class Config {
private:
    std::map<std::string, std::string> settings;
    static Config* instance;

    Config(const std::string& filepath);
    void loadEnvFile(const std::string& filepath);

public:
    static Config& getInstance(const std::string& filepath = ".env");
    std::string get(const std::string& key) const;
    bool has(const std::string& key) const;

    // Prevent copying
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};
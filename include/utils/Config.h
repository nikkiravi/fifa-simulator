// include/utils/Config.h
#pragma once
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>

/**
 * @brief Reads and exposes key–value pairs from a `.env` file (Singleton pattern).
 *
 * Config loads a `.env`-format file (one `KEY=VALUE` pair per line; lines
 * starting with `#` are treated as comments) on first construction, and
 * makes the values accessible via get() for the lifetime of the application.
 *
 * Typical usage:
 * @code
 *   Config& cfg = Config::getInstance();          // loads ".env" by default
 *   std::string apiKey = cfg.get("API_KEY");
 *   std::string baseUrl = cfg.get("API_BASE_URL");
 * @endcode
 *
 * A custom file path can be supplied on the first call:
 * @code
 *   Config& cfg = Config::getInstance("path/to/my.env");
 * @endcode
 *
 * @note Copying is disabled — only one instance should exist.
 */
class Config {
private:
    std::map<std::string, std::string> settings; ///< Parsed key–value pairs.
    static Config* instance;                     ///< Singleton instance pointer.

    Config(const std::string& filepath);

    /**
     * @brief Parses the `.env` file at @p filepath into the settings map.
     * @param filepath Path to the `.env` file.
     * @throws std::runtime_error if the file cannot be opened.
     */
    void loadEnvFile(const std::string& filepath);

public:
    /**
     * @brief Returns the singleton Config, creating it on first call.
     * @param filepath Path to the `.env` file (default: ".env").
     * @return Reference to the singleton Config.
     */
    static Config& getInstance(const std::string& filepath = ".env");

    /**
     * @brief Returns the value associated with @p key.
     * @param key The environment variable name to look up.
     * @return The associated value string.
     * @throws std::runtime_error if @p key is not present.
     */
    std::string get(const std::string& key) const;

    /**
     * @brief Checks whether @p key exists in the configuration.
     * @param key The environment variable name to check.
     * @return true if the key is present.
     */
    bool has(const std::string& key) const;

    // Copying is disabled — only one instance should ever exist.
    Config(const Config&)            = delete;
    Config& operator=(const Config&) = delete;
};
#pragma once
#include <string>
#include <nlohmann/json.hpp>

class APIClient {
private:
    std::string apiKey;
    std::string baseUrl;
    static APIClient* instance;

    APIClient(const std::string& key, const std::string& url);

public:
    static APIClient& getInstance(const std::string& key = "", const std::string& url = "");

    // HTTP GET request
    nlohmann::json get(const std::string& endpoint);

    // Prevent copying
    APIClient(const APIClient&) = delete;
    APIClient& operator=(const APIClient&) = delete;
};
#include "api/APIClient.h"
#include <cpr/cpr.h>
#include <stdexcept>
#include <iostream>

APIClient* APIClient::instance = nullptr;

APIClient& APIClient::getInstance(const std::string& key, const std::string& url) {
    if (!instance) {
        if (key.empty() || url.empty()) {
            throw std::runtime_error("APIClient requires apiKey and baseUrl on first initialization");
        }
        instance = new APIClient(key, url);
    }
    return *instance;
}

APIClient::APIClient(const std::string& key, const std::string& url)
    : apiKey(key), baseUrl(url) {
    std::cout << "APIClient initialized with base URL: " << baseUrl << std::endl;
}

nlohmann::json APIClient::get(const std::string& endpoint) {
    std::string url = baseUrl + endpoint;
    std::cout << "Fetching: " << url << std::endl;

    // Make HTTP GET request with API key in header
    cpr::Response response = cpr::Get(
        cpr::Url{url},
        cpr::Header{{"X-Auth-Token", apiKey}}
    );

    // Check HTTP status code
    if (response.status_code != 200) {
        throw std::runtime_error(
            "API request failed with status " + std::to_string(response.status_code) +
            ": " + response.text
        );
    }

    // Parse JSON response
    try {
        return nlohmann::json::parse(response.text);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("Failed to parse JSON response: " + std::string(e.what()));
    }
}
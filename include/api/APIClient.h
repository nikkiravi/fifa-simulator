// include/api/APIClient.h
#pragma once
#include <string>
#include <nlohmann/json.hpp>

/**
 * @brief HTTP client for the football-data.org REST API (Singleton pattern).
 *
 * APIClient wraps CPR to perform authenticated GET requests against the
 * football-data.org v4 API.  It is a Singleton so the API key and base URL
 * are configured once and shared throughout the application.
 *
 * Initialise on first use by supplying a key and URL:
 * @code
 *   Config& cfg = Config::getInstance();
 *   APIClient& api = APIClient::getInstance(cfg.get("API_KEY"),
 *                                           cfg.get("API_BASE_URL"));
 *   auto json = api.get("/competitions/PL/teams");
 * @endcode
 *
 * Subsequent calls to getInstance() with no arguments return the same instance.
 *
 * @note Copying is disabled.
 */
class APIClient {
private:
    std::string apiKey;  ///< football-data.org API key sent in the "X-Auth-Token" header.
    std::string baseUrl; ///< Base URL for all API requests (e.g., "https://api.football-data.org/v4").
    static APIClient* instance; ///< Singleton instance pointer.

    APIClient(const std::string& key, const std::string& url);

public:
    /**
     * @brief Returns the singleton APIClient, creating it on first call.
     *
     * @param key  API key (required on the first call; ignored on subsequent calls).
     * @param url  Base URL (required on the first call; ignored on subsequent calls).
     * @return Reference to the singleton APIClient.
     */
    static APIClient& getInstance(const std::string& key = "",
                                  const std::string& url = "");

    /**
     * @brief Performs a GET request and returns the parsed JSON response.
     *
     * Appends @p endpoint to the base URL and sends the request with the
     * API key in the "X-Auth-Token" header.
     *
     * @param endpoint Path segment to append (e.g., "/competitions/PL/teams").
     * @return Parsed JSON object.
     * @throws std::runtime_error on HTTP error or JSON parse failure.
     */
    nlohmann::json get(const std::string& endpoint);

    // Copying is disabled — only one instance should ever exist.
    APIClient(const APIClient&)            = delete;
    APIClient& operator=(const APIClient&) = delete;
};
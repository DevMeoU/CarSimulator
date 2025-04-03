// Client code (client.cpp)
#include <iostream>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

int main() {
    httplib::Client client("localhost", 8080); // Correct initialization
    client.set_connection_timeout(5);  // Tăng timeout lên 5 giây
    
    // Enhanced JSON payload
    json payload = {
        {"metadata", {
            {"timestamp", static_cast<int>(time(nullptr))},
            {"source", "cpp_client"}
        }},
        {"user", {
            {"username", "cpp_dev"},
            {"credentials", {
                {"email", "dev@cpp.com"},
                {"age", 35},
                {"skills", {"C++", "JSON", "REST"}}
            }}
        }}
    };

    // POST request with error handling
    std::cout << "\nSending POST request...\n";
    if (auto res = client.Post("/api/data", payload.dump(), "application/json")) {
        std::cout << "POST Response [" << res->status << "]:\n"
                  << json::parse(res->body).dump(4) << "\n\n";
    } else {
        std::cerr << "POST Error: " << res.error() << "\n";
    }

    // GET request with timeout
    std::cout << "Sending GET request...\n";
    client.set_connection_timeout(2);
    if (auto res = client.Get("/api/data")) {
        std::cout << "GET Response [" << res->status << "]:\n"
                  << json::parse(res->body).dump(4) << "\n";
    } else {
        std::cerr << "GET Error: " << res.error() << "\n";
    }

    return 0;
}
// Thêm vào đầu file httplib.h
#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600  // Windows Vista trở lên
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netdb.h>
#endif

// Thêm vào cuối file httplib.h
#include <iostream>
#include "httplib.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#define PORT 8080
#define HOST "localhost"

// Biến lưu trữ dữ liệu JSON từ client
extern std::string stored_json_data;
std::string stored_json_data = "";

void post_handler(const httplib::Request& req, httplib::Response& res) {
    if (!req.has_header("Content-Type") || req.get_header_value("Content-Type") != "application/json") {
        res.status = 400;
        res.set_content("Invalid Content-Type", "text/plain");
        return;
    }

    try {
        // Parse và validate JSON trước khi lưu
        json j = json::parse(req.body);
        stored_json_data = req.body;
        res.status = 200;
        res.set_header("Content-Type", "application/json");
        json response = {
            {"status", "success"},
            {"message", "Data received"}
        };
        res.set_content(response.dump(), "application/json");
    } catch (const json::parse_error& e) {
        res.status = 400;
        res.set_header("Content-Type", "application/json");
        json response = {
            {"status", "error"},
            {"message", "Invalid JSON format"}
        };
        res.set_content(response.dump(), "application/json");
    } catch (const std::exception& e) {
        res.status = 500;
        res.set_header("Content-Type", "application/json");
        json response = {
            {"status", "error"},
            {"message", "Internal server error"}
        };
        res.set_content(response.dump(), "application/json");
    }
}

void get_handler(const httplib::Request& req, httplib::Response& res) {
    res.status = 200;
    res.set_header("Content-Type", "application/json");
    res.set_content(stored_json_data.empty() ? "{}" : stored_json_data, "application/json");
}

int main() {
#ifdef _WIN32
    // Khởi tạo Winsock trên Windows
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Khởi tạo Winsock thất bại!" << endl;
        return 1;
    }
#endif

    // Tạo server
    httplib::Server svr;
    svr.set_mount_point("/", "../ui");
    svr.set_mount_point("/data", "../data");

    // Add POST handler for /data endpoint
    svr.Post("/api/data", post_handler);

    // Add GET handler for /data endpoint
    svr.Get("/api/data", get_handler);

    // User defined file extension and MIME type mappings
    svr.set_file_extension_and_mimetype_mapping("html", "text/html");
    svr.set_file_extension_and_mimetype_mapping("css", "text/css");
    svr.set_file_extension_and_mimetype_mapping("js", "application/javascript");
    svr.set_file_extension_and_mimetype_mapping("csv", "text/plain");
    svr.set_file_extension_and_mimetype_mapping("json", "application/json");
    svr.set_file_extension_and_mimetype_mapping("jpg", "image/jpeg");
    svr.set_file_extension_and_mimetype_mapping("png", "image/png");

    // Thông báo khởi động
    system("cls && chcp 65001>nul");
    cout << "Server đang chạy tại http://" << HOST << ":" << PORT << endl;
    #ifdef _WIN32
        system("start http://localhost:8080");
    #elif __APPLE__
        system("open http://localhost:8080");
    #endif
    
    svr.listen(HOST, PORT);

#ifdef _WIN32
    // Dọn dẹp Winsock
    WSACleanup();
#endif

    return 0;
}
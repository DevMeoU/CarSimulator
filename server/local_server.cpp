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
#include "./cpp-httplib/httplib.h"

using namespace std;

#define PORT 8080
#define HOST "localhost"

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

    // Biến lưu trữ dữ liệu JSON từ client
    std::string stored_json_data;

    // Add POST handler for /data endpoint
    svr.Post("/data", [&stored_json_data](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_header("Content-Type") || req.get_header_value("Content-Type") != "application/json") {
            res.status = 400;
            res.set_content("Invalid Content-Type", "text/plain");
            return;
        }

        try {
            // Lưu trữ dữ liệu JSON
            stored_json_data = req.body;
            res.status = 200;
            res.set_content("Data received", "text/plain");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content("Invalid JSON data", "text/plain");
        }
    });

    // Add GET handler for /data endpoint
    svr.Get("/data", [&stored_json_data](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
        res.set_header("Content-Type", "application/json");
        res.set_content(stored_json_data.empty() ? "{}" : stored_json_data, "application/json");
    });

    // User defined file extension and MIME type mappings
    svr.set_file_extension_and_mimetype_mapping("html", "text/html");
    svr.set_file_extension_and_mimetype_mapping("css", "text/css");
    svr.set_file_extension_and_mimetype_mapping("js", "application/javascript");
    // svr.set_file_extension_and_mimetype_mapping("csv", "text/csv");
    svr.set_file_extension_and_mimetype_mapping("csv", "text/plain");

    // Thông báo khởi động
    cout << "Server đang chạy tại http://" << HOST << ":" << PORT << endl;
    svr.listen(HOST, PORT);

#ifdef _WIN32
    // Dọn dẹp Winsock
    WSACleanup();
#endif

    return 0;
}
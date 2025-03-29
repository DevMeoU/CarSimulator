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
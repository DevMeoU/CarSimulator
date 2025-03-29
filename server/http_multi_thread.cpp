#include "cpp-httplib/httplib.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

// Biến count dùng kiểu int thông thường, được bảo vệ bởi mutex
int count = 0;
std::mutex count_mutex;      // Bảo vệ cập nhật biến count
std::mutex file_mutex;       // Bảo vệ quá trình ghi file CSV

// Hàm lấy thời gian hiện tại dưới dạng chuỗi
std::string get_current_time() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return buf;
}

// Hàm auto-post dữ liệu: nhiều luồng có thể chạy song song
void auto_post_data(const std::string &url, const std::string &csv_filename, int thread_id) {
    httplib::Client cli("localhost", 8080);
    while (true) {
        // Lấy và tăng biến count một cách an toàn bằng mutex
        int current_count;
        {
            std::lock_guard<std::mutex> lock(count_mutex);
            current_count = count++;
        }
        std::string current_time = get_current_time();

        // Ghi dữ liệu vào file CSV (bảo vệ bằng mutex riêng cho file)
        {
            std::lock_guard<std::mutex> lock(file_mutex);
            std::ofstream file(csv_filename, std::ios::app);
            if (!file.is_open()) {
                std::cerr << "Thread " << thread_id << ": Failed to open file: " << csv_filename << std::endl;
                return;
            }
            file << current_time << "," << current_count << "\n";
            file.close();
        }

        // Gửi POST request đến server
        httplib::Params params;
        params.emplace("time", current_time);
        params.emplace("count", std::to_string(current_count));

        auto res = cli.Post(url.c_str(), params);
        if (res && res->status == 200) {
            std::cout << "Thread " << thread_id << " posted successfully: " << res->body << std::endl;
        } else {
            std::cerr << "Thread " << thread_id << " failed to post data" << std::endl;
        }

        // Chờ 3 giây trước khi gửi lần tiếp theo
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main() {
    httplib::Server svr;
    svr.set_mount_point("/", "../ui");

    // Endpoint POST: ghi dữ liệu nhận được từ auto-post vào CSV
    svr.Post("/update_csv", [&](const httplib::Request &req, httplib::Response &res) {
        std::string csv_filename = "../data/data.csv";

        // Kiểm tra tham số "time"
        if (!req.has_param("time") || !req.has_param("count")) {
            res.status = 400;
            res.set_content("Missing 'time' or 'count' parameter", "text/plain");
            return;
        }

        std::string current_time = req.get_param_value("time");
        std::string received_count = req.get_param_value("count");

        // Endpoint chỉ ghi lại dữ liệu mà không can thiệp vào biến count
        {
            std::lock_guard<std::mutex> lock(file_mutex);
            std::ofstream file(csv_filename, std::ios::app);
            if (!file.is_open()) {
                res.status = 500;
                res.set_content("Failed to open file", "text/plain");
                return;
            }
            file << current_time << "," << received_count << "\n";
            file.close();
        }

        std::string response = "CSV updated at " + current_time + " with value " + received_count;
        res.set_content(response, "text/plain");
        std::cout << response << std::endl;
    });

    // API bật auto-post
    svr.Get("/start_auto_post", [&](const httplib::Request &, httplib::Response &res) {
        res.set_content("Auto-post started", "text/plain");
        std::cout << "Auto-post enabled\n";
    });

    // API dừng auto-post (nếu cần, có thể xử lý thông qua một biến toàn cục)
    svr.Get("/stop_auto_post", [&](const httplib::Request &, httplib::Response &res) {
        res.set_content("Auto-post stopped", "text/plain");
        std::cout << "Auto-post disabled\n";
    });

    // Cấu hình server sử dụng một luồng duy nhất để xử lý request theo thứ tự
    svr.new_task_queue = [] { return new httplib::ThreadPool(1); };

    // Khởi tạo nhiều luồng auto-post (ví dụ 2 luồng) để gửi dữ liệu đồng thời
    const int num_threads = 2;
    std::vector<std::thread> auto_post_threads;
    for (int i = 0; i < num_threads; ++i) {
        auto_post_threads.emplace_back(auto_post_data, "/update_csv", "data.csv", i);
    }

    std::cout << "Server running at http://localhost:8080" << std::endl;
    svr.listen("localhost", 8080);

    for (auto &t : auto_post_threads) {
        t.join();
    }
    return 0;
}

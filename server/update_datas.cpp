#include "cpp-httplib/httplib.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include <unordered_map>
#include <random>

// Mutex bảo vệ việc ghi file CSV
std::mutex file_mutex;

// Hàm tạo số ngẫu nhiên trong khoảng [min, max]
int random_int(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Hàm tạo random string cho "mode": eco hoặc sport
std::string random_mode() {
    return (random_int(0, 1) == 0 ? "eco" : "sport");
}

// Hàm tạo random string cho "gear": P, R, N, D
std::string random_gear() {
    std::vector<std::string> gears = {"P", "R", "N", "D"};
    return gears[random_int(0, gears.size()-1)];
}

// Hàm tạo dữ liệu ngẫu nhiên cho tất cả các thuộc tính
std::unordered_map<std::string, std::string> generate_random_data() {
    std::unordered_map<std::string, std::string> data;
    data["speed"] = std::to_string(random_int(0, 300));
    data["mode"] = random_mode();
    data["battery"] = std::to_string(random_int(0, 100));
    data["battery-temp"] = std::to_string(random_int(0, 100));
    data["gear"] = random_gear();
    data["estimated-distance"] = std::to_string(random_int(100, 10000));
    data["distance-traveled"] = std::to_string(random_int(100, 5000));
    data["signal-left"] = std::to_string(random_int(0, 1));
    data["signal-right"] = std::to_string(random_int(0, 1));
    data["wind"] = std::to_string(random_int(0, 5));
    data["air-condition"] = std::to_string(random_int(16, 28));
    data["plug-in"] = std::to_string(random_int(0, 1));
    data["door-lock"] = std::to_string(random_int(0, 1));
    data["seat-belt"] = std::to_string(random_int(0, 1));
    data["park"] = std::to_string(random_int(0, 1));
    return data;
}

// Hàm lấy thời gian hiện tại dưới dạng chuỗi theo định dạng ISO 8601 (không có mili giây)
std::string get_current_time() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tstruct);
    return std::string(buf);
}

// Hàm ghi dữ liệu ngẫu nhiên vào file CSV (một dòng gồm timestamp và các giá trị theo thứ tự cố định)
void write_csv(const std::string &filename, const std::unordered_map<std::string, std::string> &data) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV file: " << filename << std::endl;
        return;
    }
    // Ghi header
    file << "Key, Value,\n";
    // Ghi từng cặp key-value theo thứ tự cố định
    file << "speed, " << data.at("speed") << ",\n"
         << "mode, " << data.at("mode") << ",\n"
         << "battery, " << data.at("battery") << ",\n"
         << "battery-temp, " << data.at("battery-temp") << ",\n"
         << "gear, " << data.at("gear") << ",\n"
         << "estimated-distance, " << data.at("estimated-distance") << ",\n"
         << "distance-traveled, " << data.at("distance-traveled") << ",\n"
         << "signal-left, " << data.at("signal-left") << ",\n"
         << "signal-right, " << data.at("signal-right") << ",\n"
         << "wind, " << data.at("wind") << ",\n"
         << "air-condition, " << data.at("air-condition") << ",\n"
         << "plug-in, " << data.at("plug-in") << ",\n"
         << "door-lock, " << data.at("door-lock") << ",\n"
         << "seat-belt, " << data.at("seat-belt") << ",\n"
         << "park, " << data.at("park") << ",\n";

    file.close();
}

// Hàm gửi dữ liệu qua HTTP POST tới endpoint "/update_csv"
void post_data(const std::string &url, const std::unordered_map<std::string, std::string> &data) {
    const int MAX_RETRIES = 3;
    const int RETRY_DELAY_MS = 1000;
    
    httplib::Client cli("localhost", 8080);
    cli.set_connection_timeout(5); // 5 seconds timeout
    
    httplib::Params params;
    for (const auto &pair : data) {
        params.emplace(pair.first, pair.second);
    }
    
    for (int retry = 0; retry < MAX_RETRIES; ++retry) {
        auto res = cli.Post(url.c_str(), params);
        if (res && res->status == 200) {
            std::cout << "Posted successfully: " << res->body << std::endl;
            return;
        }
        
        std::cerr << "Failed to post data (attempt " << retry + 1 << " of " << MAX_RETRIES << ")" << std::endl;
        if (retry < MAX_RETRIES - 1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_DELAY_MS));
        }
    }
}

// Luồng thực hiện việc cập nhật: mỗi lần, nó tạo dữ liệu ngẫu nhiên, ghi file CSV và gửi HTTP POST.
void random_update_thread(const std::string &url, const std::string &csv_filename, int thread_id) {
    while (true) {
        auto data = generate_random_data();
        // Optionally, bạn có thể thêm timestamp vào data nếu muốn:
        // data["timestamp"] = get_current_time();
        write_csv(csv_filename, data);
        post_data(url, data);
        std::cout << "Thread " << thread_id << " updated data." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main() {
    // Thiết lập HTTP server để xử lý POST /update_csv, chỉ trả về phản hồi đơn giản
    httplib::Server svr;

    // Endpoint POST: ghi dữ liệu nhận được từ auto-post vào CSV
    svr.Post("/data", [&](const httplib::Request &req, httplib::Response &res) {
        std::string response = "Data received: ";
        for (const auto &p : req.params) {
            response += p.first + "=" + p.second + "; ";
        }
        res.set_content(response, "text/plain");
        std::cout << response << std::endl;
    });

    // Cấu hình server sử dụng một luồng xử lý request (để duy trì thứ tự)
    svr.new_task_queue = [] { return new httplib::ThreadPool(1); };

    // Khởi chạy server trong một luồng riêng
    std::thread server_thread([&svr]() {
        std::cout << "Server running at http://localhost:8080" << std::endl;
        svr.listen("localhost", 8080);
    });

    // Khởi tạo nhiều luồng cập nhật ngẫu nhiên
    const int num_threads = 3;
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(random_update_thread, "/data", "../data/storage.csv", i);
    }

    for (auto &t : threads) {
        t.join();
    }
    server_thread.join();

    return 0;
}

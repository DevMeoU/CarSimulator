#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include "cpp-httplib/httplib.h"

// Mutex để bảo vệ dữ liệu chia sẻ giữa các luồng, đảm bảo tính nhất quán khi nhiều luồng cùng truy cập
// Khi một luồng đang thực hiện thao tác với dữ liệu được bảo vệ, các luồng khác phải chờ đợi
std::mutex data_mutex;

// Cấu trúc dữ liệu chia sẻ: một bảng băm lưu trữ các thông số xe dưới dạng cặp key-value
// Mỗi key là một chuỗi đại diện cho một thông số xe (ví dụ: tốc độ, chế độ lái)
// Mỗi value là một chuỗi chứa giá trị tương ứng của thông số đó
std::unordered_map<std::string, std::string> share_data = {
    {"speed", "67"},
    {"mode", "eco"},
    {"battery", "15"},
    {"battery-temp", "54"},
    {"gear", "P"},
    {"estimated-distance", "00"},
    {"distance-traveled", "00"},
    {"signal-left", "1"},
    {"signal-right", "1"},
    {"wind", "5"},
    {"air-condition", "24"},
    {"plug-in", "0"},
    {"door-lock", "0"},
    {"seat-belt", "0"},
    {"park", "1"}
};

// Hàm lấy thời gian hiện tại dưới dạng chuỗi định dạng YYYY-MM-DD HH:MM:SS
// Được sử dụng để ghi log và theo dõi thời điểm cập nhật dữ liệu
std::string get_current_time() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return buf;
}

// Hàm cập nhật dữ liệu cho một key cụ thể trong share_data
// Sử dụng std::lock_guard để tự động khóa và mở khóa mutex, đảm bảo thread-safe
// Khi hàm kết thúc, lock_guard sẽ tự động giải phóng mutex
void update_data(const std::string &key, const std::string &new_value) {
    std::lock_guard<std::mutex> lock(data_mutex);
    share_data[key] = new_value;
}

// Hàm ghi toàn bộ dữ liệu từ share_data ra file CSV
// Sử dụng mutex để đảm bảo không có luồng nào đang thay đổi dữ liệu khi đang ghi file
// File CSV có định dạng: Key, Value cho mỗi dòng
void write_csv(const std::string &filename) {
    std::lock_guard<std::mutex> lock(data_mutex);
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV file." << std::endl;
        return;
    }
    // Ghi header
    file << "Key, Value,\n";
    // Ghi từng cặp key-value
    for (const auto &pair : share_data) {
        file << pair.first << ", " << pair.second << ",\n";
    }
    file.close();
    std::cout << "CSV written at " << get_current_time() << std::endl;
}

// Hàm mô phỏng việc cập nhật dữ liệu từ nhiều luồng
// Hàm tạo giá trị ngẫu nhiên cho từng thông số xe
// Mỗi thông số có một phạm vi giá trị hợp lý khác nhau:
// - Tốc độ: 0-300 km/h, mô phỏng tốc độ tối đa của xe
// - Chế độ: eco (tiết kiệm) hoặc sport (thể thao)
// - Pin và nhiệt độ pin: 0-100%, mô phỏng mức pin và nhiệt độ pin
// - Số: P (đỗ), R (lùi), N (số 0), D (số tiến)
// - Khoảng cách ước tính: 100-10000 km, dựa trên lượng pin hiện tại
// - Khoảng cách đã đi: 100-5000 km, đồng hồ công tơ mét
// - Các thông số boolean (tín hiệu, cửa, dây an toàn): 0 (tắt) hoặc 1 (bật)
// - Gió: 0-5, mức độ quạt gió điều hòa
// - Điều hòa: 16-28°C, nhiệt độ điều hòa
std::string generate_random_value(const std::string &key) {
    if (key == "speed") return std::to_string(rand() % 301); // 0-300 km/h
    if (key == "mode") return (rand() % 2 == 0) ? "eco" : "sport"; // Chế độ lái
    if (key == "battery" || key == "battery-temp") return std::to_string(rand() % 101); // 0-100%
    if (key == "gear") {
        std::string gears[] = {"P", "R", "N", "D"}; // Các chế độ số
        return gears[rand() % 4];
    }
    if (key == "estimated-distance") return std::to_string(100 + rand() % 9901); // 100-10000 km
    if (key == "distance-traveled") return std::to_string(100 + rand() % 4901); // 100-5000 km
    if (key == "signal-left" || key == "signal-right" || 
        key == "plug-in" || key == "door-lock" || 
        key == "seat-belt" || key == "park") return std::to_string(rand() % 2); // 0-1 (tắt-bật)
    if (key == "wind") return std::to_string(rand() % 6); // 0-5 (mức độ quạt)
    if (key == "air-condition") return std::to_string(16 + rand() % 13); // 16-28°C
    return "0"; // Giá trị mặc định
}

void simulate_updates(int thread_id) {
    // Lấy danh sách các key từ share_data
    std::vector<std::string> keys;
    {
        std::lock_guard<std::mutex> lock(data_mutex);
        for (const auto &pair : share_data) {
            keys.push_back(pair.first);
        }
    }
    
    while (true) {
        // Cập nhật tất cả các key
        for (const auto &key : keys) {
            std::string new_value = generate_random_value(key);
            update_data(key, new_value);
            std::cout << "Thread " << thread_id << " updated " << key << " to " << new_value << std::endl;
        }
        
        // Ghi dữ liệu ra file CSV
        write_csv("data.csv");
        
        // Gửi HTTP POST request
        httplib::Client cli("localhost", 8080);
        httplib::Params params;
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            for (const auto &pair : share_data) {
                params.emplace(pair.first, pair.second);
            }
        }
        
        auto res = cli.Post("/update", params);
        if (res && res->status == 200) {
            std::cout << "Thread " << thread_id << " posted data successfully" << std::endl;
        } else {
            std::cerr << "Thread " << thread_id << " failed to post data" << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// Hàm main khởi tạo 3 luồng mô phỏng cập nhật dữ liệu
// Mỗi luồng chạy độc lập và cập nhật dữ liệu liên tục
// Sử dụng join() để đợi tất cả các luồng hoàn thành (trong trường hợp này là vô hạn)
int main() {
    // Khởi tạo một số luồng mô phỏng cập nhật dữ liệu
    const int num_threads = 3;
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(simulate_updates, i);
    }
    
    // Chờ tất cả các luồng hoàn thành công việc
    for (auto &t : threads) {
        t.join();
    }
    
    // Sau khi cập nhật xong, ghi toàn bộ dữ liệu ra file CSV
    write_csv("data.csv");
    
    return 0;
}

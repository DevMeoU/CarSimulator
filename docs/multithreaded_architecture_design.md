# Thiết kế kiến trúc đa luồng cho hệ thống mô phỏng xe

## 1. Tổng quan kiến trúc

Hệ thống mô phỏng xe sẽ được thiết kế với 4 luồng chính:

1. **Luồng giả lập môi trường (Environment Simulation Thread)**
   - Mô phỏng các điều kiện môi trường ảnh hưởng đến xe
   - Cập nhật các thông số môi trường theo thời gian thực

2. **Luồng thực thi chính (Main Execution Thread)**
   - Quản lý vòng đời của ứng dụng
   - Khởi tạo và điều phối các luồng khác
   - Xử lý logic chính của xe

3. **Luồng xử lý sự kiện bàn phím (Keyboard Event Processing Thread)**
   - Bắt và xử lý các sự kiện bàn phím từ người dùng
   - Cập nhật trạng thái xe dựa trên đầu vào của người dùng

4. **Luồng gửi dữ liệu lên server (Server Data Posting Thread)**
   - Định kỳ gửi dữ liệu trạng thái xe lên server HTTP
   - Xử lý phản hồi từ server

## 2. Cấu trúc dữ liệu chia sẻ

Để đảm bảo tính nhất quán dữ liệu giữa các luồng, chúng ta sẽ sử dụng một cấu trúc dữ liệu chia sẻ:

```cpp
struct VehicleData {
    std::mutex mutex;
    double speed;
    std::string mode;
    double battery;
    double battery_temp;
    std::string gear;
    double estimated_distance;
    double distance_traveled;
    bool signal_left;
    bool signal_right;
    double wind;
    double air_condition;
    bool plug_in;
    bool door_lock;
    bool seat_belt;
    bool park;
    bool brake;
    bool gas;
    std::string warning;
};
```

## 3. Cơ chế đồng bộ hóa

Để đảm bảo an toàn khi truy cập dữ liệu chia sẻ, chúng ta sẽ sử dụng các cơ chế đồng bộ hóa sau:

1. **Mutex**: Bảo vệ dữ liệu chia sẻ khi đọc/ghi
2. **Atomic Variables**: Sử dụng cho các biến đơn giản cần được cập nhật nguyên tử
3. **Condition Variables**: Cho các tình huống cần đợi/thông báo giữa các luồng

## 4. Chi tiết thiết kế từng luồng

### 4.1. Luồng giả lập môi trường

```cpp
class EnvironmentThread {
private:
    std::shared_ptr<VehicleData> vehicleData;
    std::thread thread;
    std::atomic<bool> running;
    
    void threadFunction() {
        while (running) {
            // Cập nhật điều kiện môi trường
            updateEnvironmentalConditions();
            
            // Ngủ một khoảng thời gian
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    void updateEnvironmentalConditions() {
        std::lock_guard<std::mutex> lock(vehicleData->mutex);
        // Cập nhật các thông số môi trường như gió, nhiệt độ, v.v.
        // Ảnh hưởng đến hiệu suất xe dựa trên Database.md
    }
    
public:
    EnvironmentThread(std::shared_ptr<VehicleData> data) : vehicleData(data), running(false) {}
    
    void start() {
        running = true;
        thread = std::thread(&EnvironmentThread::threadFunction, this);
    }
    
    void stop() {
        running = false;
        if (thread.joinable()) {
            thread.join();
        }
    }
};
```

### 4.2. Luồng thực thi chính

```cpp
class MainThread {
private:
    std::shared_ptr<VehicleData> vehicleData;
    std::thread thread;
    std::atomic<bool> running;
    
    void threadFunction() {
        while (running) {
            // Cập nhật trạng thái xe
            updateVehicleState();
            
            // Ngủ một khoảng thời gian
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    
    void updateVehicleState() {
        std::lock_guard<std::mutex> lock(vehicleData->mutex);
        // Cập nhật các thông số xe như tốc độ, pin, khoảng cách, v.v.
        // Áp dụng các quy tắc từ Operation.md
    }
    
public:
    MainThread(std::shared_ptr<VehicleData> data) : vehicleData(data), running(false) {}
    
    void start() {
        running = true;
        thread = std::thread(&MainThread::threadFunction, this);
    }
    
    void stop() {
        running = false;
        if (thread.joinable()) {
            thread.join();
        }
    }
};
```

### 4.3. Luồng xử lý sự kiện bàn phím

```cpp
class KeyboardThread {
private:
    std::shared_ptr<VehicleData> vehicleData;
    std::thread thread;
    std::atomic<bool> running;
    
    void threadFunction() {
        while (running) {
            // Bắt sự kiện bàn phím
            int key = getKeyPress();
            
            // Xử lý sự kiện
            processKeyEvent(key);
        }
    }
    
    int getKeyPress() {
        // Triển khai bắt phím (khác nhau giữa Windows và WSL)
        // Trả về mã phím
    }
    
    void processKeyEvent(int key) {
        std::lock_guard<std::mutex> lock(vehicleData->mutex);
        
        // Xử lý các phím khác nhau
        switch (key) {
            case KEY_SPACE:
                // Xử lý ga
                vehicleData->gas = true;
                // Cập nhật tốc độ
                vehicleData->speed += calculateAcceleration();
                break;
            case KEY_B:
                // Xử lý phanh
                vehicleData->brake = true;
                vehicleData->speed -= calculateDeceleration();
                break;
            // Các phím khác...
        }
    }
    
    double calculateAcceleration() {
        // Tính toán gia tốc dựa trên chế độ lái và các yếu tố khác
    }
    
    double calculateDeceleration() {
        // Tính toán giảm tốc dựa trên các yếu tố
    }
    
public:
    KeyboardThread(std::shared_ptr<VehicleData> data) : vehicleData(data), running(false) {}
    
    void start() {
        running = true;
        thread = std::thread(&KeyboardThread::threadFunction, this);
    }
    
    void stop() {
        running = false;
        if (thread.joinable()) {
            thread.join();
        }
    }
};
```

### 4.4. Luồng gửi dữ liệu lên server

```cpp
class ServerThread {
private:
    std::shared_ptr<VehicleData> vehicleData;
    std::thread thread;
    std::atomic<bool> running;
    std::string serverUrl;
    
    void threadFunction() {
        while (running) {
            // Chuẩn bị dữ liệu
            json data = prepareData();
            
            // Gửi dữ liệu lên server
            sendDataToServer(data);
            
            // Ngủ một khoảng thời gian
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    
    json prepareData() {
        std::lock_guard<std::mutex> lock(vehicleData->mutex);
        
        json data;
        data["speed"] = vehicleData->speed;
        data["mode"] = vehicleData->mode;
        data["battery"] = vehicleData->battery;
        data["battery_temp"] = vehicleData->battery_temp;
        data["gear"] = vehicleData->gear;
        data["estimated_distance"] = vehicleData->estimated_distance;
        data["distance_traveled"] = vehicleData->distance_traveled;
        data["signal_left"] = vehicleData->signal_left ? 1 : 0;
        data["signal_right"] = vehicleData->signal_right ? 1 : 0;
        data["wind"] = vehicleData->wind;
        data["air_condition"] = vehicleData->air_condition;
        data["plug_in"] = vehicleData->plug_in ? 1 : 0;
        data["door_lock"] = vehicleData->door_lock ? 1 : 0;
        data["seat_belt"] = vehicleData->seat_belt ? 1 : 0;
        data["park"] = vehicleData->park ? 1 : 0;
        data["brake"] = vehicleData->brake ? 1 : 0;
        data["gas"] = vehicleData->gas ? 1 : 0;
        data["warning"] = vehicleData->warning;
        
        return data;
    }
    
    void sendDataToServer(const json& data) {
        // Sử dụng thư viện HTTP để gửi dữ liệu lên server
        // POST http://localhost:8080/data
    }
    
public:
    ServerThread(std::shared_ptr<VehicleData> data, const std::string& url) 
        : vehicleData(data), serverUrl(url), running(false) {}
    
    void start() {
        running = true;
        thread = std::thread(&ServerThread::threadFunction, this);
    }
    
    void stop() {
        running = false;
        if (thread.joinable()) {
            thread.join();
        }
    }
};
```

## 5. Tích hợp các luồng

```cpp
int main() {
    // Khởi tạo dữ liệu chia sẻ
    auto vehicleData = std::make_shared<VehicleData>();
    
    // Khởi tạo các luồng
    EnvironmentThread envThread(vehicleData);
    MainThread mainThread(vehicleData);
    KeyboardThread keyboardThread(vehicleData);
    ServerThread serverThread(vehicleData, "http://localhost:8080/data");
    
    // Khởi động các luồng
    envThread.start();
    mainThread.start();
    keyboardThread.start();
    serverThread.start();
    
    // Chờ tín hiệu kết thúc (có thể là phím ESC)
    waitForExitSignal();
    
    // Dừng các luồng
    envThread.stop();
    mainThread.stop();
    keyboardThread.stop();
    serverThread.stop();
    
    return 0;
}
```

## 6. Xử lý khác biệt giữa Windows và WSL

### 6.1. Bắt sự kiện bàn phím

**Windows:**
```cpp
#include <conio.h>

int getKeyPress() {
    if (_kbhit()) {
        return _getch();
    }
    return 0;
}
```

**WSL/Linux:**
```cpp
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int getKeyPress() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    return (ch != EOF) ? ch : 0;
}
```

### 6.2. Điều kiện biên dịch

```cpp
#ifdef _WIN32
    // Mã dành cho Windows
#else
    // Mã dành cho WSL/Linux
#endif
```

## 7. Kết luận

Kiến trúc đa luồng được thiết kế ở trên sẽ cho phép:

1. Mô phỏng môi trường một cách độc lập
2. Xử lý logic chính của xe
3. Phản hồi nhanh chóng với đầu vào từ bàn phím
4. Gửi dữ liệu lên server một cách định kỳ

Các cơ chế đồng bộ hóa đảm bảo tính nhất quán dữ liệu giữa các luồng, trong khi vẫn cho phép hoạt động song song để tăng hiệu suất.

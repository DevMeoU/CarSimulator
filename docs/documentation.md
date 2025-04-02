# Tài liệu hệ thống mô phỏng xe đa luồng

## Giới thiệu

Hệ thống mô phỏng xe đa luồng là một ứng dụng mô phỏng hoạt động của xe điện với các tính năng và chức năng thực tế. Hệ thống được thiết kế với kiến trúc đa luồng để mô phỏng đồng thời nhiều khía cạnh của xe, bao gồm môi trường, trạng thái xe, tương tác người dùng và giao tiếp với server.

## Kiến trúc hệ thống

Hệ thống được thiết kế với 4 luồng chính:

1. **Luồng giả lập môi trường (EnvironmentThread)**: Mô phỏng các điều kiện môi trường ảnh hưởng đến xe như thời tiết, nhiệt độ, gió, và độ cao.

2. **Luồng thực thi chính (MainThread)**: Quản lý logic chính của xe, cập nhật trạng thái xe, và áp dụng các quy tắc vận hành.

3. **Luồng xử lý sự kiện bàn phím (KeyboardThread)**: Bắt và xử lý các sự kiện bàn phím từ người dùng, cập nhật trạng thái xe dựa trên đầu vào của người dùng.

4. **Luồng gửi dữ liệu lên server (ServerThread)**: Định kỳ gửi dữ liệu trạng thái xe lên server HTTP và xử lý phản hồi từ server.

Các luồng này chia sẻ dữ liệu thông qua một cấu trúc dữ liệu chung (VehicleData) và được đồng bộ hóa bằng các cơ chế như mutex để đảm bảo tính nhất quán của dữ liệu.

## Cấu trúc mã nguồn

### Các file chính

- **VehicleData.h**: Định nghĩa cấu trúc dữ liệu chia sẻ giữa các luồng.
- **EnvironmentThread.h/cpp**: Triển khai luồng giả lập môi trường.
- **MainThread.h/cpp**: Triển khai luồng thực thi chính.
- **KeyboardThread.h/cpp**: Triển khai luồng xử lý sự kiện bàn phím.
- **ServerThread.h/cpp**: Triển khai luồng gửi dữ liệu lên server.
- **VehicleSimulation.h/cpp**: Lớp chính quản lý toàn bộ hệ thống mô phỏng xe.
- **main.cpp**: Điểm vào chính của ứng dụng.
- **Makefile**: Cấu hình biên dịch cho cả Windows và WSL/Linux.

### Các file từ mã nguồn gốc

- **Vehicle.h/cpp**: Định nghĩa lớp Vehicle đại diện cho xe.
- **Engine.h/cpp**: Định nghĩa lớp Engine đại diện cho động cơ.
- **Battery.h/cpp**: Định nghĩa lớp Battery đại diện cho pin.
- **DrivingMode.h/cpp**: Định nghĩa lớp DrivingMode đại diện cho chế độ lái.
- **SafetySystem.h/cpp**: Định nghĩa lớp SafetySystem đại diện cho hệ thống an toàn.
- **EnvironmentalCondition.h/cpp**: Định nghĩa lớp EnvironmentalCondition đại diện cho điều kiện môi trường.
- **FaultSimulation.h/cpp**: Định nghĩa lớp FaultSimulation đại diện cho mô phỏng lỗi.
- **Sensor.h/cpp**: Định nghĩa lớp Sensor đại diện cho cảm biến.
- **Display.h/cpp**: Định nghĩa lớp Display đại diện cho màn hình hiển thị.
- **Utils.h/cpp**: Các tiện ích hỗ trợ.

## Luồng dữ liệu

1. **Luồng giả lập môi trường** cập nhật các điều kiện môi trường trong cấu trúc dữ liệu chia sẻ.
2. **Luồng thực thi chính** đọc dữ liệu từ cấu trúc chia sẻ, cập nhật trạng thái xe, và ghi lại các thay đổi vào cấu trúc chia sẻ.
3. **Luồng xử lý sự kiện bàn phím** bắt các sự kiện từ người dùng và cập nhật trạng thái xe trong cấu trúc chia sẻ.
4. **Luồng gửi dữ liệu lên server** đọc dữ liệu từ cấu trúc chia sẻ và gửi lên server HTTP.

## Đồng bộ hóa

Để đảm bảo tính nhất quán của dữ liệu giữa các luồng, hệ thống sử dụng các cơ chế đồng bộ hóa sau:

1. **Mutex**: Mỗi thao tác đọc/ghi vào cấu trúc dữ liệu chia sẻ đều được bảo vệ bằng mutex để tránh xung đột.
2. **Atomic Variables**: Các biến đơn giản như trạng thái chạy được triển khai bằng atomic để đảm bảo tính nguyên tử.
3. **Thread Synchronization**: Các luồng được khởi động và dừng theo thứ tự hợp lý để tránh xung đột.

## Tương thích đa nền tảng

Hệ thống được thiết kế để chạy trên cả Windows và WSL/Linux với các tính năng tương thích:

1. **Phát hiện nền tảng**: Sử dụng các macro tiền xử lý để phát hiện nền tảng và điều chỉnh mã nguồn phù hợp.
2. **Bắt sự kiện bàn phím**: Triển khai khác nhau cho Windows (sử dụng conio.h) và WSL/Linux (sử dụng termios.h).
3. **Makefile đa nền tảng**: Tự động phát hiện hệ điều hành và điều chỉnh các tham số biên dịch.

## Hướng dẫn biên dịch và chạy

### Yêu cầu

- Trình biên dịch C++ hỗ trợ C++17 (g++ hoặc MSVC)
- Thư viện libcurl
- Thư viện pthread

### Biên dịch

```bash
# Trên cả Windows và WSL/Linux
make
```

### Chạy

```bash
# Chạy với server mặc định (http://localhost:8080/data)
./vehicle_simulation

# Chạy với server tùy chỉnh
./vehicle_simulation http://example.com/api/data
```

## Điều khiển

Hệ thống hỗ trợ các phím điều khiển sau:

- **SPACE**: Tăng tốc
- **B**: Phanh
- **P**: Bật/tắt phanh tay
- **L**: Khóa/mở cửa
- **S**: Thắt/mở dây an toàn
- **LEFT ARROW**: Bật/tắt xi-nhan trái
- **RIGHT ARROW**: Bật/tắt xi-nhan phải
- **1**: Chế độ Normal
- **2**: Chế độ Sport
- **3**: Chế độ Eco
- **4**: Chế độ Snow/Off-road
- **ESC**: Thoát

## Giao tiếp với server

Hệ thống gửi dữ liệu lên server HTTP theo định dạng JSON:

```json
{
  "speed": 24,
  "mode": "Normal",
  "battery": 95,
  "battery_temp": 30,
  "gear": "D",
  "estimated_distance": 420,
  "distance_traveled": 5,
  "signal_left": 0,
  "signal_right": 0,
  "wind": 10,
  "air_condition": 22,
  "plug_in": 0,
  "door_lock": 1,
  "seat_belt": 1,
  "park": 0,
  "brake": 0,
  "gas": 1,
  "warning": ""
}
```

## Mở rộng

Hệ thống có thể được mở rộng bằng cách:

1. Thêm các luồng mới cho các tính năng bổ sung.
2. Mở rộng cấu trúc dữ liệu chia sẻ để lưu trữ thêm thông tin.
3. Thêm các chế độ lái mới hoặc điều kiện môi trường.
4. Tích hợp với các hệ thống khác thông qua giao diện HTTP.

## Kết luận

Hệ thống mô phỏng xe đa luồng cung cấp một nền tảng mạnh mẽ để mô phỏng hoạt động của xe điện với các tính năng thực tế. Kiến trúc đa luồng cho phép mô phỏng đồng thời nhiều khía cạnh của xe, tạo ra trải nghiệm mô phỏng chân thực và hiệu quả.

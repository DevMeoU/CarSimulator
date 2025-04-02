# Hướng dẫn sử dụng hệ thống mô phỏng xe đa luồng

## Giới thiệu

Hệ thống mô phỏng xe đa luồng là một ứng dụng cho phép người dùng mô phỏng hoạt động của xe điện với các tính năng thực tế. Hướng dẫn này sẽ giúp bạn cài đặt, biên dịch và sử dụng hệ thống.

## Cài đặt

### Yêu cầu hệ thống

- Hệ điều hành: Windows hoặc Linux/WSL
- Trình biên dịch C++ hỗ trợ C++17 (g++ hoặc MSVC)
- Thư viện libcurl
- Thư viện pthread

### Cài đặt các thư viện cần thiết

**Trên Windows:**
```bash
# Sử dụng vcpkg
vcpkg install curl
```

**Trên Linux/WSL:**
```bash
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev
```

### Tải mã nguồn

Tải mã nguồn từ kho lưu trữ hoặc giải nén từ tệp đã cung cấp.

## Biên dịch

Hệ thống sử dụng Makefile để biên dịch trên cả Windows và Linux/WSL.

```bash
# Di chuyển đến thư mục mã nguồn
cd /đường/dẫn/đến/mã/nguồn

# Biên dịch
make
```

Sau khi biên dịch thành công, bạn sẽ có tệp thực thi `vehicle_simulation` (trên Linux/WSL) hoặc `vehicle_simulation.exe` (trên Windows).

## Chạy ứng dụng

### Chạy với cấu hình mặc định

```bash
# Trên Linux/WSL
./vehicle_simulation

# Trên Windows
vehicle_simulation.exe
```

### Chạy với URL server tùy chỉnh

```bash
# Trên Linux/WSL
./vehicle_simulation http://example.com/api/data

# Trên Windows
vehicle_simulation.exe http://example.com/api/data
```

## Sử dụng ứng dụng

### Giao diện dòng lệnh

Khi khởi động, ứng dụng sẽ hiển thị thông tin hệ thống và hướng dẫn điều khiển. Nhấn Enter để bắt đầu mô phỏng.

### Điều khiển xe

Sử dụng các phím sau để điều khiển xe:

- **SPACE**: Tăng tốc (giữ để tiếp tục tăng tốc)
- **B**: Phanh (giữ để tiếp tục phanh)
- **P**: Bật/tắt phanh tay
- **L**: Khóa/mở cửa
- **S**: Thắt/mở dây an toàn
- **LEFT ARROW**: Bật/tắt xi-nhan trái
- **RIGHT ARROW**: Bật/tắt xi-nhan phải
- **1**: Chuyển sang chế độ Normal
- **2**: Chuyển sang chế độ Sport
- **3**: Chuyển sang chế độ Eco
- **4**: Chuyển sang chế độ Snow/Off-road
- **ESC**: Thoát ứng dụng

### Quy trình sử dụng cơ bản

1. **Khởi động xe**:
   - Nhấn **L** để khóa cửa
   - Nhấn **S** để thắt dây an toàn
   - Nhấn **B** và giữ trong 3 giây để nhả phanh tay
   - Phanh tay sẽ tự động nhả sau khi đạp phanh đủ 3 giây

2. **Lái xe**:
   - Nhấn **SPACE** để tăng tốc
   - Nhấn **B** để phanh
   - Sử dụng các phím **1**, **2**, **3**, **4** để chuyển đổi chế độ lái
   - Sử dụng **LEFT ARROW** và **RIGHT ARROW** để bật/tắt xi-nhan

3. **Dừng xe**:
   - Nhấn **B** để giảm tốc độ về 0
   - Nhấn **P** để kéo phanh tay
   - Nhấn **ESC** để thoát ứng dụng

## Các chế độ lái

Hệ thống hỗ trợ 4 chế độ lái với các đặc điểm khác nhau:

1. **Normal**: Cân bằng giữa hiệu suất và tiết kiệm năng lượng
   - Tốc độ tối đa: 150 km/h
   - Gia tốc: Trung bình

2. **Sport**: Tăng hiệu suất, phản hồi nhanh
   - Tốc độ tối đa: 180 km/h
   - Gia tốc: Cao
   - Tiêu thụ pin nhanh hơn

3. **Eco**: Tối ưu hóa tiết kiệm năng lượng
   - Tốc độ tối đa: 120 km/h
   - Gia tốc: Thấp
   - Tiêu thụ pin chậm hơn

4. **Snow/Off-road**: Tối ưu hóa cho địa hình khó
   - Tốc độ tối đa: 80 km/h
   - Gia tốc: Rất thấp
   - Kiểm soát lực kéo tốt hơn

## Ảnh hưởng của môi trường

Hệ thống mô phỏng các điều kiện môi trường ảnh hưởng đến hiệu suất xe:

- **Nhiệt độ**: Ảnh hưởng đến hiệu suất pin và động cơ
  - Dưới 0°C: Giảm phạm vi pin 20-30%
  - Trên 35°C: Giảm hiệu suất động cơ 10%

- **Thời tiết**:
  - Mưa: Giảm ma sát lốp 30%, ảnh hưởng đến phanh và gia tốc
  - Tuyết: Giảm ma sát lốp 50%, ảnh hưởng nghiêm trọng đến phanh và gia tốc
  - Sương mù: Giảm tầm nhìn, hiển thị cảnh báo

- **Gió**: Gió mạnh làm tăng lực cản không khí, ảnh hưởng đến tiêu thụ năng lượng

## Giao tiếp với server

Hệ thống gửi dữ liệu trạng thái xe lên server HTTP theo định dạng JSON. Dữ liệu được gửi định kỳ mỗi 500ms.

Để xem dữ liệu được gửi lên server, bạn có thể sử dụng các công cụ như:
- Postman
- cURL
- Trình duyệt web (nếu server có giao diện người dùng)

## Xử lý sự cố

### Ứng dụng không biên dịch

- Kiểm tra xem đã cài đặt đầy đủ các thư viện cần thiết chưa
- Kiểm tra phiên bản trình biên dịch C++ (cần hỗ trợ C++17)
- Kiểm tra lỗi trong quá trình biên dịch và sửa các lỗi được báo cáo

### Ứng dụng không phản hồi phím

- Kiểm tra xem terminal/console có hỗ trợ đầu vào không echo không
- Trên WSL, đảm bảo đang chạy trong terminal hỗ trợ đầy đủ

### Không kết nối được với server

- Kiểm tra xem server có đang chạy không
- Kiểm tra URL server đã chính xác chưa
- Kiểm tra kết nối mạng

## Kết luận

Hệ thống mô phỏng xe đa luồng cung cấp một trải nghiệm mô phỏng chân thực về hoạt động của xe điện. Bằng cách sử dụng các điều khiển đơn giản, bạn có thể trải nghiệm các tính năng khác nhau của xe và xem cách chúng phản ứng với các điều kiện môi trường khác nhau.

Nếu bạn gặp bất kỳ vấn đề nào không được đề cập trong hướng dẫn này, vui lòng liên hệ với nhóm phát triển để được hỗ trợ.

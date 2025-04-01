# Hướng dẫn sử dụng chương trình mô phỏng xe điện

## Giới thiệu

Chương trình mô phỏng xe điện này được phát triển dựa trên tài liệu Operation.md và Database.md. Chương trình mô phỏng hoạt động của một xe điện với các chức năng như khởi động xe, chuyển chế độ lái, cảnh báo an toàn, và mô phỏng các điều kiện môi trường ảnh hưởng đến hiệu suất xe.

## Cấu trúc thư mục

```
ElectricVehicleSimulation/
├── bin/                  # Thư mục chứa file thực thi
├── include/              # Thư mục chứa các file header (.h)
├── obj/                  # Thư mục chứa các file object (.o)
├── src/                  # Thư mục chứa các file nguồn (.cpp)
└── Makefile              # File Makefile để biên dịch chương trình
```

## Yêu cầu hệ thống

- Hệ điều hành: Linux (đã được kiểm thử trên Ubuntu)
- Trình biên dịch: g++ (hỗ trợ C++17)
- Công cụ: make

## Cách biên dịch

1. Giải nén file ElectricVehicleSimulation.zip:
   ```
   unzip ElectricVehicleSimulation.zip
   ```

2. Di chuyển vào thư mục ElectricVehicleSimulation:
   ```
   cd ElectricVehicleSimulation
   ```

3. Biên dịch chương trình:
   ```
   make
   ```

4. Chạy chương trình:
   ```
   ./bin/ElectricVehicleSimulation
   ```

## Các chức năng chính

Chương trình cung cấp một menu tương tác cho phép bạn kiểm tra các chức năng khác nhau của xe điện:

1. **Kiểm tra yêu cầu khởi động xe**: Mô phỏng các điều kiện cần thiết để khởi động xe như khóa cửa và nhả phanh tay.

2. **Kiểm tra chế độ lái và giới hạn tốc độ**: Mô phỏng các chế độ lái khác nhau (Normal, Sport, Eco, Snow/Off-road) và giới hạn tốc độ tương ứng.

3. **Kiểm tra cảnh báo dây an toàn**: Mô phỏng hệ thống cảnh báo khi xe di chuyển mà không thắt dây an toàn.

4. **Kiểm tra cảnh báo tốc độ cao**: Mô phỏng hệ thống cảnh báo khi tốc độ vượt quá 120 km/h.

5. **Kiểm tra tác động của điều kiện môi trường**: Mô phỏng ảnh hưởng của các điều kiện môi trường như nhiệt độ, độ cao, thời tiết đến hiệu suất xe.

6. **Kiểm tra pin và tính toán phạm vi**: Mô phỏng hệ thống pin và tính toán phạm vi hoạt động dựa trên điều kiện hiện tại.

7. **Kiểm tra mô phỏng lỗi**: Mô phỏng các tình huống lỗi như ngắt kết nối cảm biến, quá nhiệt động cơ, sự cố pin, mất phanh.

8. **Chạy mô phỏng hoàn chỉnh**: Chạy một mô phỏng hoàn chỉnh của một chu kỳ lái xe, bao gồm khởi động, lái xe trong thành phố, lái xe trên đường cao tốc, thay đổi điều kiện môi trường, và dừng xe.

## Các class chính

1. **Vehicle**: Class chính quản lý toàn bộ hệ thống xe.
2. **Engine**: Quản lý động cơ điện.
3. **Battery**: Quản lý pin và hệ thống sạc.
4. **DrivingMode**: Quản lý các chế độ lái.
5. **SafetySystem**: Quản lý các hệ thống an toàn.
6. **EnvironmentalCondition**: Mô phỏng các điều kiện môi trường.
7. **FaultSimulation**: Mô phỏng lỗi và sự cố.
8. **Sensor**: Quản lý các cảm biến trên xe.
9. **Display**: Hiển thị thông báo và cảnh báo.

## Lưu ý

- Chương trình này là một mô phỏng và không kết nối với phần cứng thực tế.
- Các công thức tính toán được áp dụng để mô phỏng hoạt động thực tế của xe điện.
- Chương trình có thể được mở rộng để thêm các tính năng mới hoặc cải thiện các tính năng hiện có.

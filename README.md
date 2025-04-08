# Hệ thống mô phỏng xe đa luồng (Car Simulator)

## Giới thiệu

Hệ thống mô phỏng xe đa luồng là một ứng dụng mô phỏng hoạt động của xe điện với các tính năng và chức năng thực tế. Hệ thống được thiết kế với kiến trúc đa luồng để mô phỏng đồng thời nhiều khía cạnh của xe, bao gồm môi trường, trạng thái xe, tương tác người dùng và giao tiếp với server.

## Tính năng chính

- Mô phỏng hoạt động xe điện với 4 chế độ lái (Normal, Sport, Eco, Snow/Off-road)
- Giả lập môi trường và điều kiện thời tiết ảnh hưởng đến hiệu suất xe
- Giao diện người dùng trực quan với thông số kỹ thuật thời gian thực
- Hệ thống cảnh báo an toàn thông minh
- Giao tiếp với server HTTP để theo dõi trạng thái xe

## Cài đặt

### Yêu cầu hệ thống

- Hệ điều hành: Windows hoặc Linux/WSL
- Trình biên dịch C++ hỗ trợ C++17 (g++ hoặc MSVC)
- Thư viện libcurl
- Thư viện pthread

### Biên dịch và chạy

```bash
# Biên dịch
make

# Chạy với server mặc định
./vehicle_simulation

# Chạy với server tùy chỉnh
./vehicle_simulation http://example.com/api/data
```

## Điều khiển

- **SPACE**: Tăng tốc
- **B**: Phanh
- **P**: Bật/tắt phanh tay
- **L**: Khóa/mở cửa
- **S**: Thắt/mở dây an toàn
- **LEFT/RIGHT ARROW**: Xi-nhan trái/phải
- **1-4**: Chuyển đổi chế độ lái
- **ESC**: Thoát

## Cấu trúc dự án

```
├── behind/           # Mã nguồn backend
├── data/            # Dữ liệu và cấu hình
├── docs/            # Tài liệu
├── server/          # Mã nguồn server
├── ui/              # Giao diện người dùng
└── README.md        # Tài liệu này
```

## Tài liệu chi tiết

Xem thêm trong thư mục `docs/` để biết thông tin chi tiết về:

- Kiến trúc hệ thống (`documentation.md`)
- Thông số kỹ thuật (`Database.md`)
- Quy trình vận hành (`Operation.md`)
- Hướng dẫn sử dụng (`user_guide.md`)

## Đóng góp

Mọi đóng góp đều được hoan nghênh. Vui lòng tạo issue hoặc pull request để cải thiện dự án.

## Giấy phép

Dự án này được phân phối dưới giấy phép thuộc về Deesol

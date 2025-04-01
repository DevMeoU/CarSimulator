### **I. Thông số kỹ thuật cơ bản**

| **Thành phần**             | **Thông số**                                                          |
| ---------------------------------- | ----------------------------------------------------------------------------- |
| **Động cơ điện**        | Công suất: 150 kW, Mô-men xoắn cực đại: 320 Nm                         |
| **Pin Lithium-ion**          | Dung lượng: 75 kWh, Điện áp: 400 V, Phạm vi hoạt động: 450 km (NEDC) |
| **Hệ thống sạc**          | Sạc nhanh DC: 0-80% trong 30 phút, Sạc AC: 7.4 kW (8 giờ đầy pin)       |
| **Trọng lượng**           | 1800 kg (bao gồm pin)                                                        |
| **Hệ số khí động học** | Cd: 0.23                                                                      |
| **Hộp số**                 | 1 cấp (truyền thẳng)                                                       |

---

### **II. Chế độ vận hành**

| **Chế độ**               | **Mô tả**                                                                                     |
| --------------------------------- | ----------------------------------------------------------------------------------------------------- |
| **Chế độ Normal**        | Tối ưu cân bằng giữa hiệu suất và tiết kiệm năng lượng                                   |
| **Chế độ Sport**         | Tăng công suất động cơ, phản hồi ga nhạy, giảm thời gian tăng tốc 0-100 km/h (5.2 giây) |
| **Chế độ Eco**           | Giới hạn công suất, tối ưu hóa phanh tái sinh, tăng 15% phạm vi hoạt động                |
| **Chế độ Snow/Off-road** | Kiểm soát lực kéo, phân bổ mô-men xoắn đến từng bánh, giảm trượt                       |

---

### **III. Hệ thống an toàn**

| **Hệ thống**                   | **Chi tiết**                                                            |
| -------------------------------------- | ------------------------------------------------------------------------------ |
| **ABS (Phanh chống bó cứng)** | Điều chỉnh áp suất phanh 100 lần/giây, phạm vi tốc độ: 5-200 km/h   |
| **ESP (Ổn định thân xe)**    | Can thiệp khi phát hiện trượt bánh, góc lái lệch quá 15°            |
| **ADAS (Hỗ trợ lái)**         | Bao gồm: Cảnh báo va chạm trước (FCW), Tự động phanh khẩn cấp (AEB) |
| **Giám sát pin**               | Cảnh báo nhiệt độ pin vượt 45°C, ngắt sạc khi quá nhiệt (>60°C)   |
| **Airbag**                       | 6 túi khí (2 trước, 2 bên, 2 rèm)                                        |

---

### **IV. Điều kiện môi trường ảnh hưởng**

| **Yếu tố**    | **Tác động đến hiệu suất**                                                                                 |
| --------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| **Nhiệt độ** | - Dưới 0°C: Giảm 20-30% phạm vi pin, tăng thời gian sạc``- Trên 35°C: Giảm hiệu suất động cơ 10% |
| **Độ cao**    | Giảm 5% hiệu suất động cơ mỗi 1000m (do giảm mật độ không khí)                                             |
| **Địa hình** | - Đường dốc 10%: Tăng mức tiêu thụ năng lượng 25%``- Đường gồ ghề: Giảm tốc độ tối đa 20%  |
| **Tải trọng** | Thêm 100 kg: Giảm phạm vi 5-8%                                                                                       |

---

### **V. Mô phỏng lỗi và sự cố**

| **Kịch bản**                | **Phản ứng hệ thống**                                                            |
| ----------------------------------- | ------------------------------------------------------------------------------------------ |
| **Mất kết nối cảm biến** | Kích hoạt chế độ Safe Mode: Giới hạn tốc độ 50 km/h, cảnh báo trên màn hình |
| **Quá nhiệt động cơ**    | Tự động giảm công suất 50%, làm mát bằng hệ thống liquid cooling                |
| **Sự cố pin**               | Ngắt nguồn cao áp, cách ly module pin hỏng, cảnh báo sơ tán                       |
| **Mất phanh**                | Kích hoạt phanh tái sinh tối đa, sử dụng phanh điện từ khẩn cấp                |

---

### **VI. Thông số mô phỏng phần mềm**

| **Tham số**                   | **Giá trị mặc định**                                                  |
| ------------------------------------ | -------------------------------------------------------------------------------- |
| **Tần số cập nhật**        | 100 Hz (cảm biến), 50 Hz (hệ thống điều khiển)                            |
| **Độ trễ hệ thống**       | <20 ms (từ cảm biến đến phản hồi điều khiển)                           |
| **Mô hình động lực học** | Sử dụng phương trình chuyển động 6 bậc tự do (6-DOF)                   |
| **Mô phỏng thời tiết**     | Mưa (giảm ma sát lốp 30%), Tuyết (giảm 50%), Sương mù (tầm nhìn <50m) |

---

### **VII. Tiêu chuẩn an toàn áp dụng**

* **ISO 26262** (Functional Safety cho xe cơ giới)
* **UNECE R100** (An toàn pin cho xe điện)
* **NHTSA NCAP** (Đánh giá va chạm 5 sao)

---

### **Gợi ý công cụ mô phỏng**

* **Phần mềm** : MATLAB/Simulink, CarMaker, ANSYS Electromagnetics
* **Phần cứng** : HIL (Hardware-in-the-Loop) với máy tính thời gian thực (dSpace)

Bạn có thể điều chỉnh các thông số trên dựa trên yêu cầu cụ thể của dự án

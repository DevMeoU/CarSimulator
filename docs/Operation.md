
**1. **Khởi động xe

* **Yêu cầu**: Khi khởi động xe, cần đạp phanh trong 3 giây để nhả phanh tay.
* **Chi tiết**:
  * **Phanh tay (**parkingBrake**) ban đầu ở trạng thái khóa (**true**).**
  * **Người dùng phải đạp phanh liên tục trong 3 giây (được theo dõi bằng biến **brakePressTime**) để nhả phanh tay (**parkingBrake = false**).**
  * **Nếu không đạp đủ thời gian, xe sẽ không khởi động và hiển thị cảnh báo: "Press brake for 3 seconds to release parking brake".**

**2. **Giới hạn tốc độ theo chế độ

* **Yêu cầu**: Mỗi chế độ lái có giới hạn tốc độ tối đa riêng.
* **Chi tiết**:
  * **Sport**: Tốc độ tối đa là 180 km/h.
  * **Eco**: Tốc độ tối đa là 120 km/h.
  * **Snow/Off-road**: Tốc độ tối đa là 80 km/h.
  * **Normal**: Tốc độ tối đa là 150 km/h.
  * **Tốc độ hiện tại của xe (**speed**) sẽ không vượt quá giới hạn của chế độ đang chọn (**currentMaxSpeed**).**

**3. **Cảnh báo thắt dây an toàn

* **Yêu cầu**: Cảnh báo khi xe di chuyển mà không thắt dây an toàn.
* **Chi tiết**:
  * **Nếu dây an toàn (**seatbeltOn**) không được thắt (**false**) và xe đang di chuyển (**speed > 0**), hệ thống sẽ hiển thị cảnh báo: "Seatbelt not fastened".**
  * **Cảnh báo sẽ được xóa khi dây an toàn được thắt hoặc xe dừng lại.**

**4. **Tự động giảm tốc độ từ từ khi chuyển mode

* **Yêu cầu**: Khi chuyển sang chế độ có tốc độ tối đa thấp hơn, tốc độ xe giảm từ từ.
* **Chi tiết**:
  * **Khi chuyển chế độ (ví dụ: từ Sport sang Eco), nếu tốc độ hiện tại (**speed**) vượt quá giới hạn của chế độ mới (**currentMaxSpeed**), tốc độ sẽ giảm dần với tốc độ giảm 10 km/h mỗi giây cho đến khi đạt giới hạn mới.**
  * **Điều này mô phỏng hành vi thực tế của xe khi thay đổi chế độ lái.**

**5. **Cảnh báo chuyển mode ở tốc độ cao

* **Yêu cầu**: Cảnh báo nếu cố chuyển chế độ khi tốc độ vượt quá giới hạn của chế độ mới.
* **Chi tiết**:
  * **Khi xe đang di chuyển (**speed > 0**) và người dùng cố chuyển sang chế độ có tốc độ tối đa thấp hơn (ví dụ: từ Sport 180 km/h sang Eco 120 km/h), nếu tốc độ hiện tại vượt quá giới hạn của chế độ mới, hệ thống sẽ hiển thị cảnh báo: "Cannot change mode at high speed".**
  * **Việc chuyển chế độ sẽ bị chặn cho đến khi tốc độ giảm xuống dưới giới hạn mới.**

**6. **Cảnh báo tốc độ cao

* **Yêu cầu**: Cảnh báo khi tốc độ vượt quá một ngưỡng nhất định.
* **Chi tiết**:
  * **Khi tốc độ (**speed**) vượt quá 120 km/h, hệ thống sẽ hiển thị cảnh báo: "High speed warning".**
  * **Cảnh báo này áp dụng cho tất cả các chế độ lái.**

**7. **Không cho khởi động khi chưa khóa cửa

* **Yêu cầu**: Xe không thể khởi động nếu cửa chưa được khóa.
* **Chi tiết**:
  * **Trước khi khởi động động cơ (**engineOn = true**), hệ thống kiểm tra trạng thái khóa cửa (**doorLocked**).**
  * **Nếu cửa chưa khóa (**doorLocked = false**), xe sẽ không khởi động và hiển thị cảnh báo: "Doors not locked".**

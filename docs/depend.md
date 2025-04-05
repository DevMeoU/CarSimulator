### **1. Sự Phụ Thuộc Giữa Các Thông Số**

#### **a. Nhóm Động Cơ & Năng Lượng**

| Thông Số             | Phụ Thuộc Vào                    | Mô Tả                                                                                                                                          |
| ---------------------- | ----------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| `engine_power`       | `engine_torque`, `speed`        | Công suất động cơ tỷ lệ thuận với mô-men xoắn và tốc độ: P=τ⋅ω**P**=**τ**⋅**ω** (với ω**ω** tính từ `speed`).  |
| `estimated_distance` | `battery`, `speed`, `gas`     | Quãng đường ước tính phụ thuộc vào mức pin (xe điện) hoặc nhiên liệu (xe xăng), tốc độ, và chế độ vận hành (`mode`). |
| `battery_temp`       | `battery`, `engine_power`       | Nhiệt độ pin tăng khi xả sâu hoặc sạc nhanh, liên quan đến công suất động cơ.                                                    |
| `engine_temp`        | `engine_power`, `air_condition` | Nhiệt độ động cơ tăng khi hoạt động ở công suất cao hoặc sử dụng điều hòa.                                                    |

#### **b. Nhóm An Toàn**

| Thông Số         | Phụ Thuộc Vào               | Mô Tả                                                                                              |
| ------------------ | ------------------------------ | ---------------------------------------------------------------------------------------------------- |
| `brake_pressure` | `brake`, `speed`           | Áp suất phanh tăng khi đạp phanh (`brake` = true) và tốc độ cao.                          |
| `esp_active`     | `speed`, `brake`, `gear` | Hệ thống cân bằng điện tử kích hoạt khi mất lực kéo (phụ thuộc tốc độ, phanh, số). |
| `door_lock`      | `speed`                      | Cửa tự động khóa khi tốc độ vượt ngưỡng (ví dụ: >20 km/h).                             |

#### **c. Nhóm Môi Trường**

| Thông Số      | Phụ Thuộc Vào              | Mô Tả                                                                                |
| --------------- | ----------------------------- | -------------------------------------------------------------------------------------- |
| `temperature` | `engine_temp`, `weather`  | Nhiệt độ cabin phụ thuộc vào nhiệt độ động cơ và thời tiết bên ngoài. |
| `battery`     | `plug_in`, `engine_power` | Pin sạc lại nếu `plug_in` = true hoặc tụt do sử dụng công suất động cơ.  |

---

### **2. Cảnh Báo (Warnings)**

#### **a. Cảnh Báo Năng Lượng**

* **Cảnh báo pin yếu** : Khi `battery` < 20% và `plug_in` = false.
* **Quá nhiệt pin** : Khi `battery_temp` > 50°C.
* **Tiêu hao năng lượng bất thường** : Nếu `battery` giảm >5%/phút ở chế độ `mode` = "Eco".

#### **b. Cảnh Báo Động Cơ**

* **Động cơ quá nhiệt** : Khi `engine_temp` > 120°C.
* **Công suất vượt ngưỡng** : Khi `engine_power` > 150 kW liên tục trong 30 giây.

#### **c. Cảnh Báo An Toàn**

* **Phanh khẩn cấp** : Khi `brake_pressure` > 200 bar hoặc < 50 bar.
* **Chưa thắt dây an toàn** : Khi `seat_belt` = false và `speed` > 0.
* **Cửa chưa khóa** : Khi `door_lock` = false và `speed` > 20 km/h.

#### **d. Cảnh Báo Hệ Thống**

* **Lỗi ABS/ESP** : Khi `abs_active`/`esp_active` = false nhưng hệ thống phát hiện trượt bánh.
* **Cảnh báo thời tiết** : Khi `weather` = "Rain" hoặc `wind` > 15 m/s và `speed` > 80 km/h.

---

### **3. Công Thức Tính Toán**

#### **a. Ước Lượng Quãng Đường Còn Lại**

* **Xe điện** :
  estimated_distance=battery×energy_efficiency100(km)**estimated_distance**=**100**battery**×**energy_efficiency****(**km**)Trong đó:
  energy_efficiency=8−0.05×speed**energy_efficiency**=**8**−**0.05**×**speed** (kWh/100 km)
* **Xe xăng** :
  estimated_distance=gas×fuel_efficiency100(km)**estimated_distance**=**100**gas**×**fuel_efficiency****(**km**)fuel_efficiency=15−0.1×speed**fuel_efficiency**=**15**−**0.1**×**speed** (km/lít).

#### **b. Nhiệt Độ Động Cơ**

engine_temp=ambient_temp+engine_power10(°C)**engine_temp**=**ambient_temp**+**10**engine_power****(**°**C**)**(giả định `ambient_temp` từ `weather`).

#### **c. Tốc Độ Tiêu Hao Pin**

battery_drop_rate=engine_power×0.1+air_condition×260(%/phuˊt)**battery_drop_rate**=**60**engine_power**×**0.1**+**air_condition**×**2****(**%/**ph**u**ˊ**t**)---

### **4. Ghi Chú**

1. **Trùng lặp dữ liệu** : Trường `Speed` và `speed` cần kiểm tra lại để tránh nhầm lẫn.
2. **Đơn vị** : Cần xác định rõ đơn vị (ví dụ: `speed` = km/h, `brake_pressure` = bar).
3. **Cảnh báo ESP/ABS** : Hiện đang gán cứng `esp_active` = false – cần tích hợp logic thực tế (ví dụ: dựa trên cảm biến góc lái).

---

### **5. Ví Dụ Thực Tế**

* **Kịch bản 1** : Xe chạy ở tốc độ 100 km/h, `battery` = 30%, `air_condition` = true:
  estimated_distance=30×(8−0.05×100)100=9 km**estimated_distance**=**100**30**×**(**8**−**0.05**×**100**)****=**9** km
* **Kịch bản 2** : `engine_power` = 120 kW, `ambient_temp` = 25°C:
  engine_temp=25+12010=37°C**engine_temp**=**25**+**10**120****=**37°**C

Hãy điều chỉnh các hệ số và ngưỡng dựa trên dữ liệu thực tế của phương tiện!


### 📘 **Tài liệu về sự phụ thuộc giữa các thông số xe & cảnh báo thực tế**

#### 1. **Battery & Battery Temp**

* **Mối liên hệ:** Nhiệt độ ắc quy ảnh hưởng trực tiếp đến hiệu suất và tuổi thọ.
* **Cảnh báo:**
  * `battery < 20%` → Cảnh báo pin yếu.
  * `battery_temp > 50°C` → Cảnh báo quá nhiệt.
* **Công thức thực tế:**
  * **Hiệu suất pin suy giảm** :

  <pre class="overflow-visible!" data-start="689" data-end="778"><div class="contain-inline-size rounded-md border-[0.5px] border-token-border-medium relative bg-token-sidebar-surface-primary"><div class="flex items-center text-token-text-secondary px-4 py-2 text-xs font-sans justify-between h-9 bg-token-sidebar-surface-primary dark:bg-token-main-surface-secondary select-none rounded-t-[5px]">ini</div><div class="sticky top-9"><div class="absolute right-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-sidebar-surface-primary text-token-text-secondary dark:bg-token-main-surface-secondary flex items-center rounded-sm px-2 font-sans text-xs"><span class="" data-state="closed"><button class="flex gap-1 items-center select-none px-4 py-1" aria-label="Copy"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path fill-rule="evenodd" clip-rule="evenodd" d="M7 5C7 3.34315 8.34315 2 10 2H19C20.6569 2 22 3.34315 22 5V14C22 15.6569 20.6569 17 19 17H17V19C17 20.6569 15.6569 22 14 22H5C3.34315 22 2 20.6569 2 19V10C2 8.34315 3.34315 7 5 7H7V5ZM9 7H14C15.6569 7 17 8.34315 17 10V15H19C19.5523 15 20 14.5523 20 14V5C20 4.44772 19.5523 4 19 4H10C9.44772 4 9 4.44772 9 5V7ZM5 9C4.44772 9 4 9.44772 4 10V19C4 19.5523 4.44772 20 5 20H14C14.5523 20 15 19.5523 15 19V10C15 9.44772 14.5523 9 14 9H5Z" fill="currentColor"></path></svg>Copy</button></span><span class="" data-state="closed"><button class="flex items-center gap-1 px-4 py-1 select-none"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path d="M2.5 5.5C4.3 5.2 5.2 4 5.5 2.5C5.8 4 6.7 5.2 8.5 5.5C6.7 5.8 5.8 7 5.5 8.5C5.2 7 4.3 5.8 2.5 5.5Z" fill="currentColor" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"></path><path d="M5.66282 16.5231L5.18413 19.3952C5.12203 19.7678 5.09098 19.9541 5.14876 20.0888C5.19933 20.2067 5.29328 20.3007 5.41118 20.3512C5.54589 20.409 5.73218 20.378 6.10476 20.3159L8.97693 19.8372C9.72813 19.712 10.1037 19.6494 10.4542 19.521C10.7652 19.407 11.0608 19.2549 11.3343 19.068C11.6425 18.8575 11.9118 18.5882 12.4503 18.0497L20 10.5C21.3807 9.11929 21.3807 6.88071 20 5.5C18.6193 4.11929 16.3807 4.11929 15 5.5L7.45026 13.0497C6.91175 13.5882 6.6425 13.8575 6.43197 14.1657C6.24513 14.4392 6.09299 14.7348 5.97903 15.0458C5.85062 15.3963 5.78802 15.7719 5.66282 16.5231Z" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path><path d="M14.5 7L18.5 11" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path></svg>Edit</button></span></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>effective_capacity</span><span> = nominal_capacity × (</span><span>1</span><span> - </span><span>0.005</span><span> × (battery_temp - </span><span>25</span><span>))
    </span></span></code></div></div></pre>

#### 2. **Brake & Brake Pressure**

* **Mối liên hệ:** Khi phanh được nhấn, áp suất phanh tăng.
* **Cảnh báo:**
  * `brake == true` & `brake_pressure < threshold` → Cảnh báo lỗi hệ thống phanh.
  * Áp suất > 120 psi có thể gây rò rỉ hoặc lỗi ABS.

#### 3. **Engine Temp & Engine Power/Torque**

* **Mối liên hệ:** Nhiệt độ động cơ cao có thể làm giảm hiệu suất và gây hư hại.
* **Cảnh báo:**
  * `engine_temp > 105°C` → Cảnh báo quá nhiệt.
* **Giảm công suất động cơ:**
  <pre class="overflow-visible!" data-start="1252" data-end="1312"><div class="contain-inline-size rounded-md border-[0.5px] border-token-border-medium relative bg-token-sidebar-surface-primary"><div class="flex items-center text-token-text-secondary px-4 py-2 text-xs font-sans justify-between h-9 bg-token-sidebar-surface-primary dark:bg-token-main-surface-secondary select-none rounded-t-[5px]">nginx</div><div class="sticky top-9"><div class="absolute right-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-sidebar-surface-primary text-token-text-secondary dark:bg-token-main-surface-secondary flex items-center rounded-sm px-2 font-sans text-xs"><span class="" data-state="closed"><button class="flex gap-1 items-center select-none px-4 py-1" aria-label="Copy"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path fill-rule="evenodd" clip-rule="evenodd" d="M7 5C7 3.34315 8.34315 2 10 2H19C20.6569 2 22 3.34315 22 5V14C22 15.6569 20.6569 17 19 17H17V19C17 20.6569 15.6569 22 14 22H5C3.34315 22 2 20.6569 2 19V10C2 8.34315 3.34315 7 5 7H7V5ZM9 7H14C15.6569 7 17 8.34315 17 10V15H19C19.5523 15 20 14.5523 20 14V5C20 4.44772 19.5523 4 19 4H10C9.44772 4 9 4.44772 9 5V7ZM5 9C4.44772 9 4 9.44772 4 10V19C4 19.5523 4.44772 20 5 20H14C14.5523 20 15 19.5523 15 19V10C15 9.44772 14.5523 9 14 9H5Z" fill="currentColor"></path></svg>Copy</button></span><span class="" data-state="closed"><button class="flex items-center gap-1 px-4 py-1 select-none"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path d="M2.5 5.5C4.3 5.2 5.2 4 5.5 2.5C5.8 4 6.7 5.2 8.5 5.5C6.7 5.8 5.8 7 5.5 8.5C5.2 7 4.3 5.8 2.5 5.5Z" fill="currentColor" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"></path><path d="M5.66282 16.5231L5.18413 19.3952C5.12203 19.7678 5.09098 19.9541 5.14876 20.0888C5.19933 20.2067 5.29328 20.3007 5.41118 20.3512C5.54589 20.409 5.73218 20.378 6.10476 20.3159L8.97693 19.8372C9.72813 19.712 10.1037 19.6494 10.4542 19.521C10.7652 19.407 11.0608 19.2549 11.3343 19.068C11.6425 18.8575 11.9118 18.5882 12.4503 18.0497L20 10.5C21.3807 9.11929 21.3807 6.88071 20 5.5C18.6193 4.11929 16.3807 4.11929 15 5.5L7.45026 13.0497C6.91175 13.5882 6.6425 13.8575 6.43197 14.1657C6.24513 14.4392 6.09299 14.7348 5.97903 15.0458C5.85062 15.3963 5.78802 15.7719 5.66282 16.5231Z" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path><path d="M14.5 7L18.5 11" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path></svg>Edit</button></span></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>if</span><span> (engine_temp > </span><span>100</span><span>)
      engine_power *= </span><span>0</span><span>.</span><span>8</span><span>
  </span></span></code></div></div></pre>

#### 4. **Seat Belt**

* **Cảnh báo:**
  * `seat_belt == false && speed > 0` → Cảnh báo không thắt dây an toàn.

#### 5. **Doors & Speed**

* **Cảnh báo:**
  * `door_lock == false && speed > 5 km/h` → Cảnh báo cửa chưa khóa khi xe đang chạy.

#### 6. **Plug_in & Battery**

* **Mối liên hệ:** Khi xe đang cắm sạc (`plug_in == true`), pin nên tăng.
* **Cảnh báo:**
  * `plug_in == true && battery không tăng` → Cảnh báo sạc lỗi.

#### 7. **Gear & Speed**

* **Mối liên hệ:** Vận tốc không tương thích với số đang gài có thể gây hại hộp số.
* **Cảnh báo:**
  * `gear == R && speed > 10` → Cảnh báo đi lùi quá nhanh.

#### 8. **Estimated Distance & Battery**

* **Công thức ước lượng:**
  <pre class="overflow-visible!" data-start="1995" data-end="2054"><div class="contain-inline-size rounded-md border-[0.5px] border-token-border-medium relative bg-token-sidebar-surface-primary"><div class="flex items-center text-token-text-secondary px-4 py-2 text-xs font-sans justify-between h-9 bg-token-sidebar-surface-primary dark:bg-token-main-surface-secondary select-none rounded-t-[5px]">ini</div><div class="sticky top-9"><div class="absolute right-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-sidebar-surface-primary text-token-text-secondary dark:bg-token-main-surface-secondary flex items-center rounded-sm px-2 font-sans text-xs"><span class="" data-state="closed"><button class="flex gap-1 items-center select-none px-4 py-1" aria-label="Copy"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path fill-rule="evenodd" clip-rule="evenodd" d="M7 5C7 3.34315 8.34315 2 10 2H19C20.6569 2 22 3.34315 22 5V14C22 15.6569 20.6569 17 19 17H17V19C17 20.6569 15.6569 22 14 22H5C3.34315 22 2 20.6569 2 19V10C2 8.34315 3.34315 7 5 7H7V5ZM9 7H14C15.6569 7 17 8.34315 17 10V15H19C19.5523 15 20 14.5523 20 14V5C20 4.44772 19.5523 4 19 4H10C9.44772 4 9 4.44772 9 5V7ZM5 9C4.44772 9 4 9.44772 4 10V19C4 19.5523 4.44772 20 5 20H14C14.5523 20 15 19.5523 15 19V10C15 9.44772 14.5523 9 14 9H5Z" fill="currentColor"></path></svg>Copy</button></span><span class="" data-state="closed"><button class="flex items-center gap-1 px-4 py-1 select-none"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path d="M2.5 5.5C4.3 5.2 5.2 4 5.5 2.5C5.8 4 6.7 5.2 8.5 5.5C6.7 5.8 5.8 7 5.5 8.5C5.2 7 4.3 5.8 2.5 5.5Z" fill="currentColor" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"></path><path d="M5.66282 16.5231L5.18413 19.3952C5.12203 19.7678 5.09098 19.9541 5.14876 20.0888C5.19933 20.2067 5.29328 20.3007 5.41118 20.3512C5.54589 20.409 5.73218 20.378 6.10476 20.3159L8.97693 19.8372C9.72813 19.712 10.1037 19.6494 10.4542 19.521C10.7652 19.407 11.0608 19.2549 11.3343 19.068C11.6425 18.8575 11.9118 18.5882 12.4503 18.0497L20 10.5C21.3807 9.11929 21.3807 6.88071 20 5.5C18.6193 4.11929 16.3807 4.11929 15 5.5L7.45026 13.0497C6.91175 13.5882 6.6425 13.8575 6.43197 14.1657C6.24513 14.4392 6.09299 14.7348 5.97903 15.0458C5.85062 15.3963 5.78802 15.7719 5.66282 16.5231Z" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path><path d="M14.5 7L18.5 11" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path></svg>Edit</button></span></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>estimated_distance</span><span> = battery × consumption_rate
  </span></span></code></div></div></pre>

#### 9. **Altitude & Engine Power**

* **Mối liên hệ:** Lên cao → không khí loãng → công suất giảm.
* **Công thức:**
  <pre class="overflow-visible!" data-start="2174" data-end="2275"><div class="contain-inline-size rounded-md border-[0.5px] border-token-border-medium relative bg-token-sidebar-surface-primary"><div class="flex items-center text-token-text-secondary px-4 py-2 text-xs font-sans justify-between h-9 bg-token-sidebar-surface-primary dark:bg-token-main-surface-secondary select-none rounded-t-[5px]">ini</div><div class="sticky top-9"><div class="absolute right-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-sidebar-surface-primary text-token-text-secondary dark:bg-token-main-surface-secondary flex items-center rounded-sm px-2 font-sans text-xs"><span class="" data-state="closed"><button class="flex gap-1 items-center select-none px-4 py-1" aria-label="Copy"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path fill-rule="evenodd" clip-rule="evenodd" d="M7 5C7 3.34315 8.34315 2 10 2H19C20.6569 2 22 3.34315 22 5V14C22 15.6569 20.6569 17 19 17H17V19C17 20.6569 15.6569 22 14 22H5C3.34315 22 2 20.6569 2 19V10C2 8.34315 3.34315 7 5 7H7V5ZM9 7H14C15.6569 7 17 8.34315 17 10V15H19C19.5523 15 20 14.5523 20 14V5C20 4.44772 19.5523 4 19 4H10C9.44772 4 9 4.44772 9 5V7ZM5 9C4.44772 9 4 9.44772 4 10V19C4 19.5523 4.44772 20 5 20H14C14.5523 20 15 19.5523 15 19V10C15 9.44772 14.5523 9 14 9H5Z" fill="currentColor"></path></svg>Copy</button></span><span class="" data-state="closed"><button class="flex items-center gap-1 px-4 py-1 select-none"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path d="M2.5 5.5C4.3 5.2 5.2 4 5.5 2.5C5.8 4 6.7 5.2 8.5 5.5C6.7 5.8 5.8 7 5.5 8.5C5.2 7 4.3 5.8 2.5 5.5Z" fill="currentColor" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"></path><path d="M5.66282 16.5231L5.18413 19.3952C5.12203 19.7678 5.09098 19.9541 5.14876 20.0888C5.19933 20.2067 5.29328 20.3007 5.41118 20.3512C5.54589 20.409 5.73218 20.378 6.10476 20.3159L8.97693 19.8372C9.72813 19.712 10.1037 19.6494 10.4542 19.521C10.7652 19.407 11.0608 19.2549 11.3343 19.068C11.6425 18.8575 11.9118 18.5882 12.4503 18.0497L20 10.5C21.3807 9.11929 21.3807 6.88071 20 5.5C18.6193 4.11929 16.3807 4.11929 15 5.5L7.45026 13.0497C6.91175 13.5882 6.6425 13.8575 6.43197 14.1657C6.24513 14.4392 6.09299 14.7348 5.97903 15.0458C5.85062 15.3963 5.78802 15.7719 5.66282 16.5231Z" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path><path d="M14.5 7L18.5 11" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path></svg>Edit</button></span></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>power_loss</span><span> = (altitude / </span><span>1000</span><span>) × </span><span>0.03</span><span>
  </span><span>effective_power</span><span> = engine_power × (</span><span>1</span><span> - power_loss)
  </span></span></code></div></div></pre>

#### 10. **Weather & Wind**

* **Ảnh hưởng đến quãng đường và vận tốc thực tế.**
* Gió ngược (wind > 20 km/h) → tăng tiêu hao năng lượng.

#### 11. **Speed & Distance**

* **Tính quãng đường theo thời gian:**
  <pre class="overflow-visible!" data-start="2486" data-end="2528"><div class="contain-inline-size rounded-md border-[0.5px] border-token-border-medium relative bg-token-sidebar-surface-primary"><div class="flex items-center text-token-text-secondary px-4 py-2 text-xs font-sans justify-between h-9 bg-token-sidebar-surface-primary dark:bg-token-main-surface-secondary select-none rounded-t-[5px]">makefile</div><div class="sticky top-9"><div class="absolute right-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-sidebar-surface-primary text-token-text-secondary dark:bg-token-main-surface-secondary flex items-center rounded-sm px-2 font-sans text-xs"><span class="" data-state="closed"><button class="flex gap-1 items-center select-none px-4 py-1" aria-label="Copy"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path fill-rule="evenodd" clip-rule="evenodd" d="M7 5C7 3.34315 8.34315 2 10 2H19C20.6569 2 22 3.34315 22 5V14C22 15.6569 20.6569 17 19 17H17V19C17 20.6569 15.6569 22 14 22H5C3.34315 22 2 20.6569 2 19V10C2 8.34315 3.34315 7 5 7H7V5ZM9 7H14C15.6569 7 17 8.34315 17 10V15H19C19.5523 15 20 14.5523 20 14V5C20 4.44772 19.5523 4 19 4H10C9.44772 4 9 4.44772 9 5V7ZM5 9C4.44772 9 4 9.44772 4 10V19C4 19.5523 4.44772 20 5 20H14C14.5523 20 15 19.5523 15 19V10C15 9.44772 14.5523 9 14 9H5Z" fill="currentColor"></path></svg>Copy</button></span><span class="" data-state="closed"><button class="flex items-center gap-1 px-4 py-1 select-none"><svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg" class="icon-xs"><path d="M2.5 5.5C4.3 5.2 5.2 4 5.5 2.5C5.8 4 6.7 5.2 8.5 5.5C6.7 5.8 5.8 7 5.5 8.5C5.2 7 4.3 5.8 2.5 5.5Z" fill="currentColor" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"></path><path d="M5.66282 16.5231L5.18413 19.3952C5.12203 19.7678 5.09098 19.9541 5.14876 20.0888C5.19933 20.2067 5.29328 20.3007 5.41118 20.3512C5.54589 20.409 5.73218 20.378 6.10476 20.3159L8.97693 19.8372C9.72813 19.712 10.1037 19.6494 10.4542 19.521C10.7652 19.407 11.0608 19.2549 11.3343 19.068C11.6425 18.8575 11.9118 18.5882 12.4503 18.0497L20 10.5C21.3807 9.11929 21.3807 6.88071 20 5.5C18.6193 4.11929 16.3807 4.11929 15 5.5L7.45026 13.0497C6.91175 13.5882 6.6425 13.8575 6.43197 14.1657C6.24513 14.4392 6.09299 14.7348 5.97903 15.0458C5.85062 15.3963 5.78802 15.7719 5.66282 16.5231Z" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path><path d="M14.5 7L18.5 11" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path></svg>Edit</button></span></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>distance += speed × delta_time
  </span></span></code></div></div></pre>

---

Bạn muốn tài liệu này dạng Markdown, PDF hay dạng trình bày trên web (HTML)? Mình có thể hỗ trợ xuất ra bất kỳ định dạng nào. Ngoài ra, bạn có cần mô phỏng các cảnh báo bằng code hay bảng dashboard trực quan không?

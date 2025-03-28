export function updateBatteryTempIcon(batteryTempPercentage) {
    const batteryIcons = document.querySelectorAll('.battery-temp-icons i');
    
    // Kiểm tra tồn tại phần tử trước khi thao tác
    if (batteryIcons.length === 0) return;
    
    // Ẩn tất cả icon
    batteryIcons.forEach(icon => {
        if (icon) icon.style.display = 'none';
    });

    // Hiển thị icon phù hợp
    const showIcon = (index) => {
        if (batteryIcons[index]) {
            batteryIcons[index].style.display = 'inline';
            document.getElementById('battery-temp').innerText = batteryTempPercentage + '%';
        }
    };

    if (batteryTempPercentage == 100) {
        showIcon(0);
    } else if ((batteryTempPercentage > 50) && (batteryTempPercentage < 100)) {
        showIcon(1);
    } else if ((batteryTempPercentage > 20) && (batteryTempPercentage <= 50)) {
        showIcon(2);
    } else if ((batteryTempPercentage > 0) && (batteryTempPercentage <= 20)) {
        showIcon(3);
    } else {
        showIcon(4);
    }
}

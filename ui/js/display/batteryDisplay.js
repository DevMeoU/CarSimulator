export function updateBatteryIcon(batteryPercentage) {
    const batteryIcons = document.querySelectorAll('.battery-icons i');
    
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
            document.getElementById('battery-level').innerText = batteryPercentage + '%';
        }
    };

    if (batteryPercentage == 100) {
        showIcon(0);
    } else if ((batteryPercentage > 50) && (batteryPercentage < 100)) {
        showIcon(1);
    } else if ((batteryPercentage > 20) && (batteryPercentage <= 50)) {
        showIcon(2);
    } else if ((batteryPercentage > 0) && (batteryPercentage <= 20)) {
        showIcon(3);
    } else {
        showIcon(4);
    }
}

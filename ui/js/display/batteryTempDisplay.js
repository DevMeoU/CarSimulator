export default class BatteryTempDisplay {
    update(batteryTemp) {
        const batteryIcons = document.querySelectorAll('.battery-temp-icons i');
        if (batteryIcons.length === 0) return;
        
        batteryIcons.forEach(icon => {
            if (icon) icon.style.display = 'none';
        });

        const showIcon = (index) => {
            if (batteryIcons[index]) {
                batteryIcons[index].style.display = 'inline';
                // Làm tròn đến 1 chữ số thập phân và hiển thị đơn vị độ C
                document.getElementById('battery-temp').innerText = batteryTemp.toFixed(1) + '°C';
            }
        };

        // Phân loại mức nhiệt độ pin
        if (batteryTemp >= 60) showIcon(0);        // Quá nóng
        else if (batteryTemp >= 45) showIcon(1);   // Nóng
        else if (batteryTemp >= 25) showIcon(2);   // Bình thường
        else if (batteryTemp >= 10) showIcon(3);   // Mát
        else showIcon(4);                          // Lạnh
    }
}
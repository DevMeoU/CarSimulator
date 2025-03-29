export default class BatteryTempDisplay {
    update(batteryTempPercentage) {
        const batteryIcons = document.querySelectorAll('.battery-temp-icons i');
        if (batteryIcons.length === 0) return;
        
        batteryIcons.forEach(icon => {
            if (icon) icon.style.display = 'none';
        });

        const showIcon = (index) => {
            if (batteryIcons[index]) {
                batteryIcons[index].style.display = 'inline';
                document.getElementById('battery-temp').innerText = batteryTempPercentage + '%';
            }
        };

        if (batteryTempPercentage == 100) showIcon(0);
        else if (batteryTempPercentage >= 75) showIcon(1);
        else if (batteryTempPercentage >= 50) showIcon(2);
        else if (batteryTempPercentage >= 25) showIcon(3);
        else showIcon(4);
    }
}
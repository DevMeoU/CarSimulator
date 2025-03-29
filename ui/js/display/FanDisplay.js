class FanDisplay {
    constructor() {
        this.fanSpeedElement = document.querySelector('.fan-speed span');
        this.fanLevelElements = document.querySelectorAll('.fan-level');
    }

    update(speed) {
        // Cập nhật số hiển thị
        this.fanSpeedElement.textContent = speed;
        
        // Cập nhật dải màu mức gió
        this.fanLevelElements.forEach((el, index) => {
            if (index < speed) {
                el.classList.add(`level-${index + 1}`);
            } else {
                el.classList.remove(`level-${index + 1}`);
                // Thêm bo góc cho level 1 và 5 khi không active
                if (index === 0) el.style.borderRadius = '4px 0 0 4px';
                else if (index === 4) el.style.borderRadius = '0 4px 4px 0';
                else el.style.borderRadius = '0';
            }
        });
    }
}

export default FanDisplay;
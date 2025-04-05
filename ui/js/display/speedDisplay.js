class SpeedDisplay {
    constructor() {
        this.speedDisplay = document.querySelector('.speed-indicator');
    }

    init() {
        // Initialize speed display if needed
        this.speedDisplay = document.querySelector('.speed-indicator');
    }

    update(speed) {
        if (this.speedDisplay) {
            // Làm tròn tốc độ đến 1 chữ số thập phân và thêm đơn vị km/h
            this.speedDisplay.textContent = `${speed.toFixed(1)}`;
        } else {
            console.error('ERROR: #speed-indicator element not found in DOM');
            console.log('Check HTML for element with this class');
        }
    }
}

export default SpeedDisplay;
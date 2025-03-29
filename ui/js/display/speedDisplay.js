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
            this.speedDisplay.textContent = speed;
        } else {
            console.error('ERROR: #speed-indicator element not found in DOM');
            console.log('Check HTML for element with this class');
        }
    }
}

export default SpeedDisplay;
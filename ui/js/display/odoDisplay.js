class OdoDisplay {
    constructor() {
        this.odometer = 0;
        this.display = document.getElementById('odometer');
    }

    // Update odometer reading
    update(distance) {
        this.odometer += distance;
        this.render();
    }

    // Reset odometer to 0
    reset() {
        this.odometer = 0;
        this.render();
    }

    // Display current odometer reading
    render() {
        if (this.display) {
            // Format to 1 decimal place
            const formattedDistance = this.odometer.toFixed(1);
            this.display.textContent = `${formattedDistance} km`;
        }
    }

    // Get current odometer value
    getValue() {
        return this.odometer;
    }
}

export default OdoDisplay;

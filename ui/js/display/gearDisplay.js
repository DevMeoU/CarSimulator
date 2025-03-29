class GearDisplay {
    constructor() {
        this.gearIndicators = {
            'P': document.querySelector('.gear-p-indicator'),
            'R': document.querySelector('.gear-r-indicator'),
            'N': document.querySelector('.gear-n-indicator'),
            'D': document.querySelector('.gear-d-indicator')
        };

        this.GEAR_COLORS = {
            'D': '#4CAF50',
            'P': '#1B10B5', 
            'N': '#FFF',
            'R': '#FF0000'
        };
    }

    update(gear) {
        // Remove active class from all indicators
        Object.values(this.gearIndicators).forEach(indicator => {
            if (indicator) {
                indicator.classList.remove('active');
                indicator.style.color = '#FFF';
            }
        });

        // Set active class and color for the selected gear
        const selectedIndicator = this.gearIndicators[gear];
        if (selectedIndicator) {
            selectedIndicator.classList.add('active');
            selectedIndicator.style.color = '#2ecc71' || '#FFF';
        } else {
            console.error('ERROR: Gear indicator not found for gear:', gear);
        }
    }
}

export default GearDisplay;
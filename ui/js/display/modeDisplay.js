class ModeDisplay {
    constructor() {
        this.ecoModeDisplay = document.getElementsByClassName('mode-eco-indicator')[0];
        this.sportModeDisplay = document.getElementsByClassName('mode-sport-indicator')[0];
    }

    update(mode) {
        if (this.ecoModeDisplay && this.sportModeDisplay) {
            if (mode === 'eco') {
                this.ecoModeDisplay.classList.add('active');
            } else {
                this.ecoModeDisplay.classList.remove('active');
            }
            
            if (mode === 'sport') {
                this.sportModeDisplay.classList.add('active'); 
            } else {
                this.sportModeDisplay.classList.remove('active'); 
            }
        }
    }
}

export default ModeDisplay;
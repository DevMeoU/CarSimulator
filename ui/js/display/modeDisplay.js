class ModeDisplay {
    constructor() {
        this.modeDisplay = document.querySelector('.mode-text');
    }

    update(mode) {
        if (this.modeDisplay) {
            this.modeDisplay.textContent = mode.toUpperCase();
            this.modeDisplay.classList.add('active');

        }
    }
}

export default ModeDisplay;
import ModeDisplay from '../display/ModeDisplay.js';

class ModeControl {
    constructor() {
        this.currentMode = 'eco';
        this.modeDisplay = new ModeDisplay();
        this.updateModeDisplay();
    }

    handleModeChange(mode, share_data) {
        if (mode === 'eco' || mode === 'sport') {
            this.currentMode = mode;
            share_data.mode = mode;
            this.updateModeDisplay();
            return true;
        }
        return false;
    }

    updateModeDisplay() {
        this.modeDisplay.update(this.currentMode);
    }

    getCurrentMode() {
        return this.currentMode;
    }
}

export default ModeControl;
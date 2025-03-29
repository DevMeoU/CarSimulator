import ModeControl from '../controls/modeControl.js';

const modeControl = new ModeControl();

// Function to handle and update mode state
export function updateMode(mode, share_data) {
    if (modeControl.handleModeChange(mode, share_data)) {
        return true;
    }
    return false;
}
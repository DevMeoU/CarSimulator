export function initModeDisplay(mode) {
    const modeDisplay = document.getElementById('mode-display');
    if (modeDisplay && mode) {
        modeDisplay.textContent = mode;
    }
}
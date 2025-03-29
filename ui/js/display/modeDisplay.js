export function initModeDisplay(mode) {
    const ecoModeDisplay = document.getElementsByClassName('mode-eco-indicator')[0];
    const sportModeDisplay = document.getElementsByClassName('mode-sport-indicator')[0];
    if (ecoModeDisplay && sportModeDisplay) {
        if (mode === 'eco') {
            ecoModeDisplay.classList.add('active');
        } else {
            ecoModeDisplay.classList.remove('active');
        }
        
        if (mode === 'sport') {
            sportModeDisplay.classList.add('active'); 
        } else {
            sportModeDisplay.classList.remove('active'); 
        }
    }
}
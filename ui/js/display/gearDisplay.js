export function initGearDisplay(gear) {
    const gearDisplay = document.querySelector('.gear-indicator');
    if (gearDisplay) {
        gearDisplay.textContent = gear;
        const GEAR_COLORS = {
            'D': '#4CAF50',
            'P': '#1B10B5',
            'N': '#FFF',
            'R': '#FF0000'
        };
        gearDisplay.style.color = GEAR_COLORS[gear] || '#FFF';
    } else {
        console.error('ERROR: #gear-indicator element not found in DOM');
        console.log('Check HTML for element with this class');
    }
}
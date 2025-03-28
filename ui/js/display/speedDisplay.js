export function initSpeedDisplay(speed) {
    const speedDisplay = document.querySelector('.speed-indicator');
    if (speedDisplay) {
        speedDisplay.textContent = speed;
    } else {
        console.error('ERROR: #speed-indicator element not found in DOM');
        console.log('Check HTML for element with this class');
    }
}
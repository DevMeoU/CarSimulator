export function updateODO(currentGear) {
    const odoElement = document.querySelector('.odo');
    odoElement.textContent = `ODO: ${currentGear === 'D' ? 'DRIVE' : currentGear} 00 km`;
}
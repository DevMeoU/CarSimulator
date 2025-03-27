import OdoDisplay from '../display/odoDisplay.js';

export function initGearControl() {
    let currentGear = 'P';

    document.addEventListener('keydown', (e) => {
        const gearKeys = {'p':'P','r':'R','n':'N','d':'D'};
        if(Object.keys(gearKeys).includes(e.key.toLowerCase())) {
            e.preventDefault();
            currentGear = gearKeys[e.key.toLowerCase()];
            updateGearDisplay();
        }
    });

    function updateGearDisplay() {
        const gearElement = document.querySelector('.gear-indicator');
        gearElement.textContent = currentGear;
            const GEAR_COLORS = {
            'D': '#4CAF50',
            'P': '#1B10B5',
            'N': '#FFF',
            'R': '#FF0000'
        };
        gearElement.style.color = GEAR_COLORS[currentGear] || '#FFF';
    }

}
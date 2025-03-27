import { updateODO } from './odoDisplay.js';

export function initGearControl() {
    let currentGear = 'P';

    document.addEventListener('keydown', (e) => {
        const gearKeys = {'p':'P','r':'R','n':'N','d':'D'};
        if(Object.keys(gearKeys).includes(e.key.toLowerCase())) {
            currentGear = gearKeys[e.key.toLowerCase()];
            updateGearDisplay();
            updateODO(currentGear);
        }
    });

    function updateGearDisplay() {
        const gearElement = document.querySelector('.gear-indicator');
        gearElement.textContent = currentGear;
        currentGear === 'D'? gearElement.style.color = '#4CAF50' : 
        currentGear === 'P'? gearElement.style.color = '#1B10B5' :
        currentGear === 'N'? gearElement.style.color = '#FFF' :
        currentGear === 'R'? gearElement.style.color = '#FF0000' : '#FFF';
    }

}
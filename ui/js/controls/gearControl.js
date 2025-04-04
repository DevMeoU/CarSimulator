import GearDisplay from '../display/GearDisplay.js';
import OdoDisplay from '../display/OdoDisplay.js';

class GearControl {
    constructor() {
        this.currentGear = 'P';
        this.gearDisplay = new GearDisplay();
        this.initKeyboardEvents();
        this.updateGearDisplay();
    }

    initKeyboardEvents() {
        document.addEventListener('keydown', (e) => {
            const gearKeys = {'p':'P','r':'R','n':'N','d':'D'};
            if(Object.keys(gearKeys).includes(e.key.toLowerCase())) {
                e.preventDefault();
                this.currentGear = gearKeys[e.key.toLowerCase()];
                this.updateGearDisplay();
            }
        });
    }

    updateGearDisplay() {
        this.gearDisplay.update(this.currentGear);
    }

    getCurrentGear() {
        return this.currentGear;
    }
}

export default GearControl;
import { updateTime } from './time.js';
import { initSignalControl } from './signalControl.js';
import { initGearControl } from './gearControl.js';
import { updateODO } from './odoDisplay.js';


document.addEventListener('DOMContentLoaded', () => {
    initGearControl();
    initSignalControl();
    updateTime();
});
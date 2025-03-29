import DisplayManager from '../managers/DisplayManager.js';
import DataManager from '../managers/DataManager.js';
import { updateMode } from './modeHandler.js';

const displayManager = new DisplayManager();
const dataManager = new DataManager();

export default dataManager.getData();
export const default_data = dataManager.getData();

export const updateShareData = (data) => {
    const updatedData = dataManager.updateData(data);
    displayManager.updates(updatedData);
    updateMode(updatedData.mode, updatedData);
}


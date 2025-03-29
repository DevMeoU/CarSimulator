import { updateTime } from '../time/time.js';
import { initCsvHandler } from '../handlers/csvHandler.js';
import { updateShareData, default_data } from '../handlers/shareJsonData.js';

class AppManager {
    constructor() {
        this.initApp();
        this.startCsvHandler();
    }

    initApp() {
        updateTime();
        updateShareData(default_data);
    }

    startCsvHandler() {
        setInterval(() => initCsvHandler(), 1000);
    }
}

export default AppManager;
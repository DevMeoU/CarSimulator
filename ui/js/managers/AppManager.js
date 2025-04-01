import { updateTime } from '../time/time.js';
import { initCsvHandler } from '../handlers/csvHandler.js';
import { responseData } from '../handlers/fetchingData.js';
import { updateShareData, default_data } from '../handlers/shareJsonData.js';

class AppManager {
    constructor() {
        this.interval = 200;
        this.initApp();
        this.startFetchingData(this.interval);
    }

    initApp() {
        updateTime();
        updateShareData(default_data);
    }

    startCsvHandler() {
        setInterval(() => initCsvHandler(), 1000);
    }

    startFetchingData(interval = 100) {
        setInterval(() => responseData(), interval);
    }
}

export default AppManager;
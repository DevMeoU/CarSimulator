// Using core-js features directly without import
import { updateTime } from './time/time.js';
import { initCsvHandler } from './handlers/csvHandler.js';

function initMain() {
    updateTime();
    import('./handlers/shareJsonData.js').then((module) => {
        module.updateShareData(module.default_data);
    });
}

// Gọi hàm xử lý dữ liệu ở đây
initMain();
setInterval(() => initCsvHandler(), 1000);

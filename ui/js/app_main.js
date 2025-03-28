import { updateTime } from './time/time.js';
import { initCsvHandler } from './handlers/csvHandler.js';

// Gọi hàm xử lý dữ liệu ở đây
document.addEventListener('DOMContentLoaded', () => {
    updateTime();
    // Gọi hàm xử lý dữ liệu ở đây
    initCsvHandler();
});
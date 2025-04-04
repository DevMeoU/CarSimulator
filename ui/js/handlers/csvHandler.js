import { updateShareData } from './ShareJsonData.js';

async function initCsvHandler() {
    try {
        let parsedData = await csvDetect();
        console.log('Dữ liệu CSV:', parsedData);
        let jsonData = csvJson(parsedData);
        console.log('Dữ liệu Json:', jsonData);
        updateShareData(jsonData);
    } catch (error) {
        console.error('Lỗi xử lý CSV:', error);
    }
}

export { initCsvHandler };

function parseCSV(csvString) {
    // Xoá BOM (nếu có)
    csvString = csvString.replace(/^\uFEFF/, '');

    // Tách chuỗi theo dòng (newline) và lọc bỏ các dòng trống
    const lines = csvString
        .split(/\r?\n/)
        .map(line => line.trim())
        .filter(line => line !== '');

    // Tách mỗi dòng theo dấu phẩy, trim khoảng trắng
    const result = lines.map(line => line.split(',').map(item => item.trim()));

    return result;
}

async function csvDetect() {
    const MAX_RETRIES = 3;
    const RETRY_DELAY = 1000; // 1 second

    for (let attempt = 1; attempt <= MAX_RETRIES; attempt++) {
        try {
            const response = await fetch('/data/storage.csv'
                , {
                   method: 'GET', 
                }
            );
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            const text = await response.text();
            return parseCSV(text);
        } catch (error) {
            console.error(`Lần thử ${attempt}/${MAX_RETRIES} - Lỗi khi tải file CSV:`, error);
            if (attempt === MAX_RETRIES) {
                throw new Error(`Không thể tải file CSV sau ${MAX_RETRIES} lần thử`);
            }
            await new Promise(resolve => setTimeout(resolve, RETRY_DELAY));
        }
    }
}

const convertToJson = (parsedData) => {
    if (!parsedData?.length) return [];
    if (!Array.isArray(parsedData)) return [];
    const headers = parsedData[0] || [];
    if (!headers.length) return [];
    return parsedData.slice(1).map(row => {
        return headers.reduce((obj, header, index) => {
            obj[header] = (row[index] || '').trim();
            return obj;
        }, {});
    });
};

function csvJson(parsedData) {
    const jsonData = convertToJson(parsedData);
    return jsonData;
}
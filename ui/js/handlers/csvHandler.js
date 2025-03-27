import { updateShareData } from './shareJsonData.js';

function initCsvHandler() {
    // Đăng ký sự kiện khi người dùng chọn file
    document.addEventListener('keydown', async (e) => {
    try {
        let parsedData = await csvDetect();
        console.log('Dữ liệu CSV:', parsedData);
        let jsonData = csvJson(parsedData);
        console.log('Dữ liệu Json:', jsonData);
        updateShareData(jsonData);
    } catch (error) {
        console.error('Lỗi xử lý CSV:', error);
    }
    });
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

function csvDetect() {
    return fetch('../data/storage.csv')
    .then(response => {
        if (!response.ok) {
        throw new Error('Network response was not ok');
        }
        return response.text();
    })
    .then(text => {
        const parsedData = parseCSV(text);
        return parsedData;
    })
    .catch(error => {
        console.error('Có lỗi xảy ra khi tải file CSV:', error);
        throw error;
    }); 
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
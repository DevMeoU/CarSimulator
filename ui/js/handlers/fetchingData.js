import { updateShareData } from './shareJsonData.js';

class DataQueue {
    constructor(maxSize = 100) {
        this.queue = [];
        this.maxSize = maxSize;
    }

    enqueue(data) {
        if (this.queue.length >= this.maxSize) {
            console.warn('Queue đã đầy, loại bỏ dữ liệu cũ nhất');
            this.queue.shift();
        }
        this.queue.push(data);
    }

    dequeue() {
        return this.queue.shift();
    }

    isEmpty() {
        return this.queue.length === 0;
    }

    size() {
        return this.queue.length;
    }
}

const dataQueue = new DataQueue();

export async function responseData() {
    try {
        let parsedData = await fetchData();
        if (parsedData) {
            console.log('Dữ liệu String:', parsedData);
            let jsonData = csvJson(parsedData);
            console.log('Dữ liệu Json:', jsonData);
            dataQueue.enqueue(jsonData);
            processQueue();
        }
    } 
    catch (error) {
        console.error('Lỗi xử lý dữ liệu JSON:', error);
    }
}

async function processQueue() {
    if (!dataQueue.isEmpty()) {
        const data = dataQueue.dequeue();
        try {
            await updateShareData(data);
            console.log('Đã xử lý dữ liệu từ queue thành công');
            if (!dataQueue.isEmpty()) {
                processQueue();
            }
        } catch (error) {
            console.error('Lỗi khi xử lý dữ liệu từ queue:', error);
            dataQueue.enqueue(data); // Thêm lại vào queue để thử lại sau
        }
    }
}

// Function to fetch data from the API
async function fetchData() {
    const MAX_RETRIES = 3;
    const RETRY_DELAY = 1000; // 1 second

    for (let attempt = 1; attempt <= MAX_RETRIES; attempt++) {
        try {
            const response = await fetch('/data', {
                method: 'GET'
            });
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            const text = await response.text();
            return text;
        } catch (error) {
            console.error(`Lần thử ${attempt}/${MAX_RETRIES} - Lỗi khi tải file JSON:`, error);
            if (attempt === MAX_RETRIES) {
                throw new Error(`Không thể tải file JSON sau ${MAX_RETRIES} lần thử`);
            }
            await new Promise(resolve => setTimeout(resolve, RETRY_DELAY));
        }
    }
    return null;
}

function csvJson(parsedData) {
    try {
        const jsonData = JSON.parse(parsedData);
        return jsonData;
    } catch (error) {
        console.error('Error parsing JSON data:', error);
        return [];
    }
}
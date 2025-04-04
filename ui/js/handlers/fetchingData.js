import { updateShareData } from './ShareJsonData.js';

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
            let jsonData = await csvJson(parsedData);
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
            const response = await fetch('/api/data', {
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

async function csvJson(parsedData) {
    try {
        // Parse JSON data
        let result;
        if (typeof parsedData === 'string') {
            result = JSON.parse(parsedData);
        } else {
            result = parsedData;
        }

        // Ensure all expected fields are present with correct types
        const expectedFields = {
            abs_active: false,
            air_condition: 0.0,
            altitude: 0.0,
            battery: 0.0,
            battery_temp: 0.0,
            brake: false,
            brake_pressure: 0.0,
            distance_traveled: 0.0,
            door_lock: false,
            engine_power: 0.0,
            engine_temp: 0.0,
            engine_torque: 0.0,
            esp_active: false,
            estimated_distance: 0.0,
            gas: false,
            gear: "",
            mode: "",
            park: false,
            plug_in: false,
            seat_belt: false,
            signal_left: false,
            signal_right: false,
            speed: 0.0,
            temperature: 0.0,
            timestamp: 0,
            warning: "",
            weather: "",
            wind: 0.0
        };

        // Ensure all fields exist with correct types
        Object.keys(expectedFields).forEach(key => {
            if (!(key in result)) {
                result[key] = expectedFields[key];
            }
        });

        return result;
    } catch (error) {
        console.error('Lỗi xử lý dữ liệu JSON:', error);
        return {};
    }
}
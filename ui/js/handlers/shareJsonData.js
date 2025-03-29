import { updateBatteryIcon } from '../display/batteryDisplay.js';
import { updateBatteryTempIcon } from '../display/batteryTemperatureDisplay.js';
import { initSignalDisplay } from '../display/signalDisplay.js';
import { initSpeedDisplay } from '../display/speedDisplay.js';
import { initGearDisplay } from '../display/gearDisplay.js';
import { initPedalDisplay } from '../display/pedalDisplay.js';
import { initOdoDisplay } from '../display/odoDisplay.js';
import { initModeDisplay } from '../display/modeDisplay.js';

var share_data = {
    speed: 0,
    mode: "eco",
    battery: 100,
    battery_temp: 25,
    gear: 'N',
    estimated_distance: 0,
    distance_traveled: 0,
    signal_left: 0,
    signal_right: 0,
    wind: 0,
    air_condition: 0,
    plug_in: 0,
    door_lock: 0,
    seat_belt: 0,
    parking_brake: 0,
    brake: 0,
    gas: 0
}
export default share_data;
export const default_data = share_data;

// Bản đồ chuyển đổi tên khóa từ data sang share_data
const keyMapping = {
    "battery-temp": "battery_temp",
    "estimated-distance": "estimated_distance",
    "distance-traveled": "distance_traveled",
    "signal-left": "signal_left",
    "signal-right": "signal_right",
    "air-condition": "air_condition",
    "plug-in": "plug_in",
    "door-lock": "door_lock",
    "seat-belt": "seat_belt",
    "brake": "brake",
    "gas": "gas",
    "parking-brake": "parking_brake"
};

function mappingData(data) {
    // Duyệt qua mảng data và gán giá trị vào share_data
    data.forEach(item => {
        let key = item.Key;
        let value = item.Value;
    
        // Nếu key có trong keyMapping, chuyển đổi tên khóa
        if (keyMapping.hasOwnProperty(key)) {
            key = keyMapping[key];
        }
        
        // Nếu share_data có khóa này và nó kiểu số, chuyển value từ chuỗi sang số
        if (share_data.hasOwnProperty(key)) {
            if (typeof share_data[key] === 'number') {
                share_data[key] = parseFloat(value);
            } else {
                share_data[key] = value;
            }
        } else {
            // Nếu share_data chưa có khóa này, bạn có thể thêm nó nếu cần
            share_data[key] = value;
        }
    });
}

export const updateShareData = (data) => {
    if(data != share_data)
    {
        mappingData(data);
    }   
    console.log('Share_data', share_data);
    // document.querySelector('.speed').innerText = share_data.speed;
    
    updateBatteryIcon(share_data.battery);
    updateBatteryTempIcon(share_data.battery_temp);
    initSignalDisplay(share_data.signal_left, share_data.signal_right);
    initSpeedDisplay(share_data.speed);
    initGearDisplay(share_data.gear);
    initPedalDisplay(share_data.brake, share_data.gas);
    initOdoDisplay(share_data.distance_traveled, share_data.estimated_distance);
    initModeDisplay(share_data.mode);
}


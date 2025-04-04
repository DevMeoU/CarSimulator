class DataManager {
    constructor() {
        this.share_data = {
            abs_active: false,
            air_condition: 0,
            altitude: 0,
            battery: 0,
            battery_temp: 0,
            brake: false,
            brake_pressure: 0,
            distance_traveled: 0,
            door_lock: false,
            engine_power: 0,
            engine_temp: 0,
            engine_torque: 0,
            esp_active: false,
            estimated_distance: 0,
            gas: false,
            gear: '',
            mode: '',
            park: false,
            plug_in: false,
            seat_belt: false,
            signal_left: false,
            signal_right: false,
            speed: 0,
            temperature: 0,
            timestamp: 0,
            warning: '',
            weather: '',
            wind: 0
        };

        this.keyMapping = {
            "abs_active": "abs_active",
            "air_condition": "air_condition",
            "altitude": "altitude",
            "battery": "battery",
            "battery_temp": "battery_temp",
            "brake": "brake",
            "brake_pressure": "brake_pressure",
            "distance_traveled": "distance_traveled",
            "door_lock": "door_lock",
            "engine_power": "engine_power",
            "engine_temp": "engine_temp",
            "engine_torque": "engine_torque",
            "esp_active": "esp_active",
            "estimated_distance": "estimated_distance",
            "gas": "gas",
            "gear": "gear",
            "mode": "mode",
            "park": "park",
            "plug_in": "plug_in",
            "seat_belt": "seat_belt",
            "signal_left": "signal_left",
            "signal_right": "signal_right",
            "speed": "speed",
            "temperature": "temperature",
            "timestamp": "timestamp",
            "warning": "warning",
            "weather": "weather",
            "wind": "wind"
        };
    }

    getData() {
        return this.share_data;
    }

    mappingData(data) {
        if ((!Array.isArray(data) || data.length === 0) && typeof(data) === "object") return data;
        
        const item = data[0];
        for (const [key, value] of Object.entries(item)) {
            const mappedKey = this.keyMapping[key] || key;
            if (this.share_data.hasOwnProperty(mappedKey)) {
                if (value === null || value === undefined) {
                    this.share_data[mappedKey] = typeof this.share_data[mappedKey] === 'number' ? 0 : '';
                    continue;
                }
                
                const defaultType = typeof this.share_data[mappedKey];
                if (defaultType === 'boolean') {
                    this.share_data[mappedKey] = Boolean(value);
                } else if (defaultType === 'number') {
                    const numValue = typeof value === 'number' ? value : parseFloat(value || '0');
                    this.share_data[mappedKey] = isNaN(numValue) ? 0 : numValue;
                } else if (mappedKey === 'gear') {
                    const validGears = ['P', 'R', 'N', 'D'];
                    const gearValue = value.toString().toUpperCase();
                    this.share_data[mappedKey] = validGears.includes(gearValue) ? gearValue : '';
                } else {
                    this.share_data[mappedKey] = value.toString();
                }
            }
        }
    }

    updateData(data) {
        if(data !== this.share_data) {
            this.share_data = this.mappingData(data);
        }   
        console.log('Share_data', this.share_data);
        return this.share_data;
    }
}

export default DataManager;
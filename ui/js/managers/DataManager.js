class DataManager {
    constructor() {
        this.share_data = {
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
            gas: 0,
            warning: 0,
        };

        this.keyMapping = {
            "speed": "speed",
            "mode": "mode",
            "battery": "battery",
            "battery-temp": "battery_temp",
            "gear": "gear",
            "estimated-distance": "estimated_distance",
            "distance-traveled": "distance_traveled",
            "signal-left": "signal_left",
            "signal-right": "signal_right",
            "wind": "wind",
            "air-condition": "air_condition",
            "plug-in": "plug_in",
            "door-lock": "door_lock",
            "seat-belt": "seat_belt",
            "brake": "brake",
            "gas": "gas",
            "parking-brake": "parking_brake",
            "warning": "warning",
        };
    }

    getData() {
        return this.share_data;
    }

    mappingData(data) {
        if (!Array.isArray(data) || data.length === 0) return;
        
        const item = data[0];
        for (const [key, value] of Object.entries(item)) {
            const mappedKey = this.keyMapping[key] || key;
            if (this.share_data.hasOwnProperty(mappedKey)) {
                if (value === null || value === undefined) {
                    this.share_data[mappedKey] = typeof this.share_data[mappedKey] === 'number' ? 0 : '';
                    continue;
                }
                
                if (mappedKey === 'gear') {
                    const validGears = ['P', 'R', 'N', 'D'];
                    const gearValue = value.toString().toUpperCase();
                    this.share_data[mappedKey] = validGears.includes(gearValue) ? gearValue : 'N';
                } else if (typeof this.share_data[mappedKey] === 'number') {
                    const numValue = typeof value === 'number' ? value : parseFloat(value || '0');
                    this.share_data[mappedKey] = isNaN(numValue) ? 0 : numValue;
                } else {
                    this.share_data[mappedKey] = value.toString();
                }
            }
        }
    }

    updateData(data) {
        if(data !== this.share_data) {
            this.mappingData(data);
        }   
        console.log('Share_data', this.share_data);
        return this.share_data;
    }
}

export default DataManager;
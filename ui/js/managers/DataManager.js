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
        data.forEach(item => {
            let key = item.Key;
            let value = item.Value;
        
            if (this.keyMapping.hasOwnProperty(key)) {
                key = this.keyMapping[key];
            }
            
            if (this.share_data.hasOwnProperty(key)) {
                if (typeof this.share_data[key] === 'number') {
                    this.share_data[key] = parseFloat(value);
                } else {
                    this.share_data[key] = value;
                }
            } else {
                this.share_data[key] = value;
            }
        });
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
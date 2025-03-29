class AirConditionDisplay {
    constructor() {
        this.airConditionLevel = document.querySelector('.air-condition-level span');
    }

    update(airCondition) {
        this.airConditionLevel.textContent = airCondition;
        
        // Thay đổi màu sắc dựa trên nhiệt độ
        if (airCondition > 28) {
            this.airConditionLevel.className = 'hot';
        } else if (airCondition < 20) {
            this.airConditionLevel.className = 'cold';
        } else {
            this.airConditionLevel.className = 'normal';
        }
    }
}

export default AirConditionDisplay;
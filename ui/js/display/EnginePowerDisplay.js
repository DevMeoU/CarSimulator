class EnginePowerDisplay {
    constructor() {
        this.powerElement = document.getElementById('engine-power');
        this.torqueElement = document.getElementById('engine-torque');
        this.tempElement = document.getElementById('engine-temp');
    }

    update(power, torque, temp) {
        if (this.powerElement) {
            // Làm tròn công suất đến 1 chữ số thập phân
            this.powerElement.textContent = `${power.toFixed(1)} kW`;
        }
        if (this.torqueElement) {
            // Làm tròn mô-men xoắn đến 1 chữ số thập phân
            this.torqueElement.textContent = `${torque.toFixed(1)} Nm`;
        }
        if (this.tempElement) {
            // Làm tròn nhiệt độ đến 1 chữ số thập phân
            this.tempElement.textContent = `${temp.toFixed(1)}°C`;
        }
    }
}

export default EnginePowerDisplay;
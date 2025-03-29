class PedalDisplay {
    constructor() {
        this.brakePedal = document.getElementById('brake-pedal');
        this.gasPedal = document.getElementById('gas-pedal');
    }

    update(brake, gas) {
        if (this.brakePedal) {
            brake ? this.brakePedal.classList.add('active') : this.brakePedal.classList.remove('active');
        }

        if (this.gasPedal) {
            gas ? this.gasPedal.classList.add('active') : this.gasPedal.classList.remove('active');
        }
    }
}

export default PedalDisplay;
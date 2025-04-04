class SafetySystemDisplay {
    constructor() {
        this.absElement = document.getElementById('abs-status');
        this.espElement = document.getElementById('esp-status');
    }

    update(absActive, espActive) {
        if (this.absElement) {
            this.absElement.classList.toggle('active', absActive);
        }
        if (this.espElement) {
            this.espElement.classList.toggle('active', espActive);
        }
    }
}

export default SafetySystemDisplay;
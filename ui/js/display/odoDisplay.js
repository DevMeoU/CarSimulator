class OdoDisplay {
    constructor() {
        this.odoTraveled = document.getElementsByClassName('distance-travelled-indicator')[0];
        this.odoEstimated = document.getElementsByClassName('distance-estimate-indicator')[0];
    }

    update(traveled, estimated) {
        if (this.odoTraveled && typeof traveled !== 'undefined') {
            this.odoTraveled.textContent = traveled.toString();
        }

        if (this.odoEstimated && typeof estimated !== 'undefined') {
            this.odoEstimated.textContent = estimated.toString();
        }
    }
}

export default OdoDisplay;

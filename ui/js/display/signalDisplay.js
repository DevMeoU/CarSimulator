class SignalDisplay {
    constructor() {
        this.signalLeft = document.querySelector('#signal-left');
        this.signalRight = document.querySelector('#signal-right');
    }

    update(signalLeft, signalRight) {
        // Chuyển đổi boolean thành số nguyên
        const leftSignal = signalLeft === true ? 1 : 0;
        const rightSignal = signalRight === true ? 1 : 0;

        // Check event listener conflicts
        if (leftSignal === 1 && rightSignal === 1) {
            if (this.signalLeft && this.signalRight) {
                this.signalLeft.classList.toggle('active', true);
                this.signalRight.classList.toggle('active', true);
            } else {
                console.error('ERROR: Signal elements not found in DOM');
            }
            return;
        }

        // Handle left signal
        if (this.signalLeft) {
            this.signalLeft.classList.toggle('active', leftSignal === 1);
        } else {
            console.error('ERROR: #signal-left element not found in DOM');
        }

        // Handle right signal
        if (this.signalRight) {
            this.signalRight.classList.toggle('active', rightSignal === 1);
        } else {
            console.error('ERROR: #signal-right element not found in DOM');
        }
    }
}

export default SignalDisplay;
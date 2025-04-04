import SignalDisplay from '../display/SignalDisplay.js';

class SignalControl {
    constructor() {
        this.isLeftSignalActive = false;
        this.isRightSignalActive = false;
        this.signalDisplay = new SignalDisplay();
        this.initKeyboardEvents();
    }

    initKeyboardEvents() {
        document.addEventListener('keydown', (e) => {
            console.log(`[DEBUG] Key pressed: ${e.key}, Code: ${e.code}`);
            
            if (e.key.toLowerCase() === 'v') {
                e.stopImmediatePropagation();
                e.preventDefault();
                this.toggleLeftSignal();
            } else if (e.key.toLowerCase() === 'b') {
                e.stopImmediatePropagation();
                e.preventDefault();
                this.toggleRightSignal();
            }
        });
    }

    toggleLeftSignal() {
        this.isLeftSignalActive = !this.isLeftSignalActive;
        this.updateSignalDisplay();
    }

    toggleRightSignal() {
        this.isRightSignalActive = !this.isRightSignalActive;
        this.updateSignalDisplay();
    }

    updateSignalDisplay() {
        this.signalDisplay.update(
            this.isLeftSignalActive ? 1 : 0,
            this.isRightSignalActive ? 1 : 0
        );
    }

    getSignalStates() {
        return {
            left: this.isLeftSignalActive,
            right: this.isRightSignalActive
        };
    }
}

export default SignalControl;
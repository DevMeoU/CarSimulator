export function initSignalDisplay(signalLeft, signalRight) {
    // Check event listener conflicts
    if (signalLeft === 1 && signalRight === 1) {
        const signal_left = document.querySelector('#signal-left');
        const signal_right = document.querySelector('#signal-right');
        if (signal_left && signal_right) {
            signal_left.classList.toggle('active', true);
            signal_right.classList.toggle('active', true);
        } else {
            console.error('ERROR: #signal-left element not found in DOM');
            console.log('Check HTML for element with this class');
        }
        return;
    }

    if (signalLeft === 1) {
        const signal = document.querySelector('#signal-left');
        if (signal) {
            signal.classList.toggle('active', true);
        } else {
            console.error('ERROR: #signal-left element not found in DOM');
            console.log('Check HTML for element with this class');
        }
    } else {
        const signal_left = document.querySelector('#signal-left.active');
        if (signal_left) {
            signal_left.classList.toggle('active', false); 
        }
    }

    if (signalRight === 1) {
        const signal = document.querySelector('#signal-right');
        if (signal) {
            signal.classList.toggle('active', true);
        } else {
            console.error('ERROR: #signal-right element not found in DOM');
            console.log('Check HTML for element with this class');
        }
    } else {
        const signal_right = document.querySelector('#signal-right.active');
        if (signal_right) {
           signal_right.classList.toggle('active', false); 
        }
    }

    return;
}
export function initSignalControl() {
    console.log('[SIGNAL] Initializing signal system');

    let isLeftSignalActive = false;
    let isRightSignalActive = false;

    // Check event listener conflicts
    console.log('[SIGNAL] Registering keydown event listener');
    document.addEventListener('keydown', handleSignalKeyPress);

    function handleSignalKeyPress(e){
        console.log(`[DEBUG] Key pressed: ${e.key}, Code: ${e.code}`);
        console.log(`Signal states before processing - Left: ${isLeftSignalActive}, Right: ${isRightSignalActive}`);
    
        if (e.key.toLowerCase() === 'v') {
            e.stopImmediatePropagation();
            e.preventDefault();
            isLeftSignalActive = !isLeftSignalActive;
            const signal = document.querySelector('.signal-left');
            if (signal) {
                signal.classList.toggle('active', isLeftSignalActive);
            } else {
                console.error('ERROR: .signal-left element not found in DOM');
                console.log('Check HTML for element with this class');
            }
        } else if (e.key.toLowerCase() === 'b') {
            e.stopImmediatePropagation();
            e.preventDefault();
            isRightSignalActive = !isRightSignalActive;
            const signal = document.querySelector('.signal-right');
            if (signal) {
                signal.classList.toggle('active', isRightSignalActive);
            } else {
                console.error('ERROR: .signal-right element not found in DOM');
                console.log('Check HTML for element with this class');
            }
        }
    };
}
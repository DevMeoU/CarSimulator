// Controls JavaScript
document.addEventListener('DOMContentLoaded', function() {
    // Initialize controls
    initializeControls();
    
    // Add event listeners for keyboard controls
    addKeyboardListeners();
    
    // Add event listeners for touch/click controls
    addTouchListeners();
});

// Initialize controls
function initializeControls() {
    // Set initial gear to P
    setGear('p');
    
    // Set initial drive mode to ECO
    setDriveMode('eco');
    
    // Set initial handbrake state to active
    setHandbrake(true);
    
    // Set initial door lock state to locked
    setDoorLock(true);
}

// Add keyboard listeners
function addKeyboardListeners() {
    document.addEventListener('keydown', function(event) {
        handleKeyDown(event);
    });
    
    document.addEventListener('keyup', function(event) {
        handleKeyUp(event);
    });
}

// Handle key down events
function handleKeyDown(event) {
    switch(event.key.toLowerCase()) {
        case 'w':
        case 'arrowup':
            startAcceleration();
            break;
        case 's':
        case 'arrowdown':
            startBraking();
            break;
        case 'p':
            setGear('p');
            break;
        case 'r':
            setGear('r');
            break;
        case 'n':
            setGear('n');
            break;
        case 'd':
            setGear('d');
            break;
        case 'e':
            setDriveMode('eco');
            break;
        case 't':
        case 's':
            setDriveMode('sport');
            break;
        case 'c':
            setDriveMode('classic');
            break;
        case 'h':
        case ' ':
            toggleHandbrake();
            break;
        case 'l':
            toggleDoorLock();
            break;
        case 'f1':
            showKeyboardOverlay();
            break;
        case 'escape':
            toggleMenu();
            break;
        case 'f11':
            toggleFullscreen();
            break;
    }
}

// Handle key up events
function handleKeyUp(event) {
    switch(event.key.toLowerCase()) {
        case 'w':
        case 'arrowup':
            stopAcceleration();
            break;
        case 's':
        case 'arrowdown':
            stopBraking();
            break;
    }
}

// Add touch listeners
function addTouchListeners() {
    // Gear buttons
    const gears = ['p', 'r', 'n', 'd'];
    gears.forEach(gear => {
        const element = document.getElementById(`gear-${gear}`);
        if (element) {
            element.addEventListener('click', function() {
                setGear(gear);
            });
        }
    });
    
    // Drive mode buttons
    const modes = ['sport', 'classic', 'eco'];
    modes.forEach(mode => {
        const element = document.getElementById(`mode-${mode}`);
        if (element) {
            element.addEventListener('click', function() {
                setDriveMode(mode);
            });
        }
    });
    
    // Handbrake button
    const handbrakeButton = document.getElementById('handbrake');
    if (handbrakeButton) {
        handbrakeButton.addEventListener('click', toggleHandbrake);
    }
    
    // Door lock button
    const doorLockButton = document.getElementById('door-lock');
    if (doorLockButton) {
        doorLockButton.addEventListener('click', toggleDoorLock);
    }
    
    // Menu items
    const menuItems = ['help', 'settings', 'about', 'exit'];
    menuItems.forEach(item => {
        const element = document.getElementById(`menu-${item}`);
        if (element) {
            element.addEventListener('click', function() {
                handleMenuItemClick(item);
            });
        }
    });
    
    // Close keyboard overlay button
    const closeKeyboardButton = document.getElementById('close-keyboard');
    if (closeKeyboardButton) {
        closeKeyboardButton.addEventListener('click', hideKeyboardOverlay);
    }
}

// Start acceleration
function startAcceleration() {
    if (window.simulationState) {
        window.simulationState.accelerating = true;
        updateStatusDisplay('Đang tăng tốc');
    }
}

// Stop acceleration
function stopAcceleration() {
    if (window.simulationState) {
        window.simulationState.accelerating = false;
        updateStatusDisplay('');
    }
}

// Start braking
function startBraking() {
    if (window.simulationState) {
        window.simulationState.braking = true;
        updateStatusDisplay('Đang phanh');
    }
}

// Stop braking
function stopBraking() {
    if (window.simulationState) {
        window.simulationState.braking = false;
        updateStatusDisplay('');
    }
}

// Set gear
function setGear(gear) {
    const gears = ['p', 'r', 'n', 'd'];
    
    gears.forEach(g => {
        const element = document.getElementById(`gear-${g}`);
        if (element) {
            if (g === gear) {
                element.classList.add('active');
            } else {
                element.classList.remove('active');
            }
        }
    });
    
    if (window.simulationState) {
        window.simulationState.currentGear = gear;
    }
    
    updateStatusDisplay(`Số: ${gear.toUpperCase()}`);
}

// Set drive mode
function setDriveMode(mode) {
    const modes = ['sport', 'classic', 'eco'];
    
    modes.forEach(m => {
        const element = document.getElementById(`mode-${m}`);
        if (element) {
            if (m === mode) {
                element.classList.add('active');
            } else {
                element.classList.remove('active');
            }
        }
    });
    
    if (window.simulationState) {
        window.simulationState.driveMode = mode;
    }
    
    updateStatusDisplay(`Chế độ lái: ${mode.charAt(0).toUpperCase() + mode.slice(1)}`);
}

// Toggle handbrake
function toggleHandbrake() {
    if (window.simulationState) {
        const newState = !window.simulationState.handbrakeActive;
        setHandbrake(newState);
    }
}

// Set handbrake state
function setHandbrake(active) {
    const handbrakeButton = document.getElementById('handbrake');
    
    if (handbrakeButton) {
        if (active) {
            handbrakeButton.classList.add('active');
            updateStatusDisplay('Phanh tay đang kéo');
        } else {
            handbrakeButton.classList.remove('active');
            updateStatusDisplay('Phanh tay đã nhả');
        }
    }
    
    if (window.simulationState) {
        window.simulationState.handbrakeActive = active;
    }
}

// Toggle door lock
function toggleDoorLock() {
    if (window.simulationState) {
        const newState = !window.simulationState.doorLocked;
        setDoorLock(newState);
    }
}

// Set door lock state
function setDoorLock(locked) {
    const doorLockButton = document.getElementById('door-lock');
    
    if (doorLockButton) {
        if (locked) {
            doorLockButton.classList.add('active');
            updateStatusDisplay('Cửa đã khóa');
        } else {
            doorLockButton.classList.remove('active');
            updateStatusDisplay('Cửa đã mở khóa');
        }
    }
    
    if (window.simulationState) {
        window.simulationState.doorLocked = locked;
    }
}

// Show keyboard overlay
function showKeyboardOverlay() {
    const overlay = document.getElementById('keyboard-overlay');
    if (overlay) {
        overlay.classList.add('visible');
    }
}

// Hide keyboard overlay
function hideKeyboardOverlay() {
    const overlay = document.getElementById('keyboard-overlay');
    if (overlay) {
        overlay.classList.remove('visible');
    }
}

// Toggle menu
function toggleMenu() {
    const menu = document.getElementById('menu');
    if (menu) {
        menu.classList.toggle('visible');
    }
}

// Handle menu item click
function handleMenuItemClick(item) {
    switch(item) {
        case 'help':
            showKeyboardOverlay();
            break;
        case 'settings':
            // Show settings panel (to be implemented)
            updateStatusDisplay('Cài đặt (chưa triển khai)');
            break;
        case 'about':
            // Show about information (to be implemented)
            updateStatusDisplay('Phiên bản 1.0.0');
            break;
        case 'exit':
            if (confirm('Bạn có chắc chắn muốn thoát?')) {
                window.close();
            }
            break;
    }
    
    // Hide menu after clicking an item
    toggleMenu();
}

// Toggle fullscreen
function toggleFullscreen() {
    if (!document.fullscreenElement) {
        document.documentElement.requestFullscreen().catch(err => {
            console.error(`Error attempting to enable full-screen mode: ${err.message}`);
        });
    } else {
        if (document.exitFullscreen) {
            document.exitFullscreen();
        }
    }
}

// Update status display
function updateStatusDisplay(message) {
    const statusElement = document.getElementById('status');
    if (statusElement && message) {
        statusElement.textContent = message;
    }
}

// Add warning message
function addWarning(message) {
    const warningsElement = document.getElementById('warnings');
    if (warningsElement) {
        const warningElement = document.createElement('div');
        warningElement.className = 'warning';
        warningElement.textContent = message;
        
        warningsElement.appendChild(warningElement);
        
        // Remove warning after 5 seconds
        setTimeout(() => {
            warningElement.remove();
        }, 5000);
    }
}

// Initialize simulation state
window.simulationState = {
    currentSpeed: 0,
    batteryLevel: 60,
    accelerating: false,
    braking: false,
    handbrakeActive: true,
    doorLocked: true,
    currentGear: 'p',
    driveMode: 'eco'
};

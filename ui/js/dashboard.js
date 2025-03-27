// Dashboard JavaScript
document.addEventListener('DOMContentLoaded', function() {
    // Initialize analog gauges
    initializeSpeedometer();
    initializeBatteryGauge();
    
    // Initialize digital dashboard
    updateDigitalTime();
    updateDigitalDate();
    
    // Update time display
    setInterval(updateTime, 1000);
    setInterval(updateDigitalTime, 1000);
});

// Initialize speedometer gauge
function initializeSpeedometer() {
    const markingsElement = document.getElementById('speed-markings');
    const numbersElement = document.getElementById('speed-numbers');
    
    // Create markings
    let markingsHTML = '';
    for (let i = 0; i <= 260; i += 10) {
        const angle = -135 + (i * 270 / 260);
        const isMajor = i % 20 === 0;
        const length = isMajor ? 15 : 10;
        
        markingsHTML += `
            <div class="gauge-marking" style="
                position: absolute;
                width: ${isMajor ? 3 : 1}px;
                height: ${length}px;
                background-color: ${isMajor ? 'rgba(255, 255, 255, 0.8)' : 'rgba(255, 255, 255, 0.5)'};
                transform-origin: bottom center;
                bottom: 50%;
                left: 50%;
                transform: translateX(-50%) rotate(${angle}deg);
            "></div>
        `;
        
        // Add numbers for major markings
        if (isMajor && i <= 260) {
            const numberAngle = angle * Math.PI / 180;
            const radius = 100; // Distance from center
            const x = Math.sin(numberAngle) * radius;
            const y = -Math.cos(numberAngle) * radius;
            
            numbersHTML += `
                <div style="
                    position: absolute;
                    top: 50%;
                    left: 50%;
                    transform: translate(${x}px, ${y}px);
                    text-align: center;
                ">${i}</div>
            `;
        }
    }
    
    markingsElement.innerHTML = markingsHTML;
    
    // Create numbers
    let numbersHTML = '';
    for (let i = 0; i <= 260; i += 20) {
        const angle = -135 + (i * 270 / 260);
        const numberAngle = angle * Math.PI / 180;
        const radius = 100; // Distance from center
        const x = Math.sin(numberAngle) * radius;
        const y = -Math.cos(numberAngle) * radius;
        
        numbersHTML += `
            <div style="
                position: absolute;
                top: 50%;
                left: 50%;
                transform: translate(${x}px, ${y}px);
                text-align: center;
            ">${i}</div>
        `;
    }
    
    numbersElement.innerHTML = numbersHTML;
}

// Initialize battery gauge
function initializeBatteryGauge() {
    const markingsElement = document.getElementById('battery-markings');
    const numbersElement = document.getElementById('battery-numbers');
    
    // Create markings
    let markingsHTML = '';
    for (let i = 0; i <= 100; i += 5) {
        const angle = -135 + (i * 270 / 100);
        const isMajor = i % 10 === 0;
        const length = isMajor ? 15 : 10;
        
        markingsHTML += `
            <div class="gauge-marking" style="
                position: absolute;
                width: ${isMajor ? 3 : 1}px;
                height: ${length}px;
                background-color: ${isMajor ? 'rgba(255, 255, 255, 0.8)' : 'rgba(255, 255, 255, 0.5)'};
                transform-origin: bottom center;
                bottom: 50%;
                left: 50%;
                transform: translateX(-50%) rotate(${angle}deg);
            "></div>
        `;
    }
    
    markingsElement.innerHTML = markingsHTML;
    
    // Create numbers
    let numbersHTML = '';
    for (let i = 0; i <= 100; i += 10) {
        const angle = -135 + (i * 270 / 100);
        const numberAngle = angle * Math.PI / 180;
        const radius = 100; // Distance from center
        const x = Math.sin(numberAngle) * radius;
        const y = -Math.cos(numberAngle) * radius;
        
        numbersHTML += `
            <div style="
                position: absolute;
                top: 50%;
                left: 50%;
                transform: translate(${x}px, ${y}px);
                text-align: center;
            ">${i}</div>
        `;
    }
    
    numbersElement.innerHTML = numbersHTML;
}

// Update speedometer
function updateSpeedometer(speed) {
    const needle = document.getElementById('speed-needle');
    const speedValue = document.getElementById('speed-value');
    const digitalSpeed = document.getElementById('digital-speed');
    
    // Calculate needle angle (-135 to 135 degrees for 0 to 260 km/h)
    const angle = -135 + (speed * 270 / 260);
    
    // Update needle position
    needle.style.transform = `translate(-50%, -100%) rotate(${angle}deg)`;
    
    // Update speed value
    speedValue.textContent = Math.round(speed);
    digitalSpeed.textContent = Math.round(speed);
}

// Update battery gauge
function updateBatteryGauge(percentage) {
    const needle = document.getElementById('battery-needle');
    const batteryValue = document.getElementById('battery-value');
    const batteryPercent = document.getElementById('battery-percent');
    const batteryFill = document.getElementById('battery-fill');
    
    // Calculate needle angle (-135 to 135 degrees for 0 to 100%)
    const angle = -135 + (percentage * 270 / 100);
    
    // Update needle position
    needle.style.transform = `translate(-50%, -100%) rotate(${angle}deg)`;
    
    // Update battery value
    batteryValue.textContent = Math.round(percentage);
    batteryPercent.textContent = Math.round(percentage) + '%';
    batteryFill.style.width = percentage + '%';
    
    // Update battery status icon
    const batteryStatusIcon = document.getElementById('status-battery');
    if (percentage < 20) {
        batteryStatusIcon.className = 'status-icon inactive';
        document.getElementById('warnings').innerHTML = '<div class="warning">Pin yếu</div>';
    } else {
        batteryStatusIcon.className = 'status-icon active';
        document.getElementById('warnings').innerHTML = '';
    }
}

// Update time display
function updateTime() {
    const now = new Date();
    const hours = now.getHours().toString().padStart(2, '0');
    const minutes = now.getMinutes().toString().padStart(2, '0');
    document.getElementById('time').textContent = `${hours}:${minutes}`;
}

// Update digital time
function updateDigitalTime() {
    const now = new Date();
    const hours = now.getHours().toString().padStart(2, '0');
    const minutes = now.getMinutes().toString().padStart(2, '0');
    document.getElementById('digital-time').textContent = `${hours}:${minutes}`;
}

// Update digital date
function updateDigitalDate() {
    const now = new Date();
    const days = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];
    const day = days[now.getDay()];
    
    const date = now.getDate().toString().padStart(2, '0');
    const month = (now.getMonth() + 1).toString().padStart(2, '0');
    const year = now.getFullYear();
    
    document.getElementById('digital-day').textContent = day;
    document.getElementById('digital-date').textContent = `${date}.${month}.${year}`;
}

// Drive mode selection
document.addEventListener('DOMContentLoaded', function() {
    const driveModes = document.querySelectorAll('.drive-mode');
    
    driveModes.forEach(mode => {
        mode.addEventListener('click', function() {
            // Remove active class from all modes
            driveModes.forEach(m => m.classList.remove('active'));
            
            // Add active class to clicked mode
            this.classList.add('active');
            
            // Update status based on selected mode
            const modeName = this.querySelector('.drive-mode-name').textContent;
            document.getElementById('status').textContent = `Chế độ lái: ${modeName}`;
        });
    });
});

// Simulate car movement
let currentSpeed = 0;
let batteryLevel = 60;
let accelerating = false;
let braking = false;
let handbrakeActive = true;

// Keyboard controls
document.addEventListener('keydown', function(event) {
    switch(event.key.toLowerCase()) {
        case 'w':
        case 'arrowup':
            accelerating = true;
            break;
        case 's':
        case 'arrowdown':
            braking = true;
            break;
        case 'h':
        case ' ':
            toggleHandbrake();
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
            setDriveMode('sport');
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
    }
});

document.addEventListener('keyup', function(event) {
    switch(event.key.toLowerCase()) {
        case 'w':
        case 'arrowup':
            accelerating = false;
            break;
        case 's':
        case 'arrowdown':
            braking = false;
            break;
    }
});

// Toggle handbrake
function toggleHandbrake() {
    handbrakeActive = !handbrakeActive;
    const handbrakeButton = document.getElementById('handbrake');
    
    if (handbrakeActive) {
        handbrakeButton.classList.add('active');
        document.getElementById('status').textContent = 'Phanh tay đang kéo';
    } else {
        handbrakeButton.classList.remove('active');
        document.getElementById('status').textContent = 'Phanh tay đã nhả';
    }
}

// Set gear
function setGear(gear) {
    const gears = ['p', 'r', 'n', 'd'];
    
    gears.forEach(g => {
        const gearElement = document.getElementById(`gear-${g}`);
        if (g === gear) {
            gearElement.classList.add('active');
        } else {
            gearElement.classList.remove('active');
        }
    });
    
    document.getElementById('status').textContent = `Số: ${gear.toUpperCase()}`;
}

// Set drive mode
function setDriveMode(mode) {
    const modes = document.querySelectorAll('.drive-mode');
    
    modes.forEach(m => {
        if (m.id === `mode-${mode}`) {
            m.classList.add('active');
        } else {
            m.classList.remove('active');
        }
    });
    
    document.getElementById('status').textContent = `Chế độ lái: ${mode.charAt(0).toUpperCase() + mode.slice(1)}`;
}

// Toggle door lock
function toggleDoorLock() {
    const doorLockButton = document.getElementById('door-lock');
    const isLocked = doorLockButton.classList.contains('active');
    
    if (isLocked) {
        doorLockButton.classList.remove('active');
        document.getElementById('status').textContent = 'Cửa đã mở khóa';
    } else {
        doorLockButton.classList.add('active');
        document.getElementById('status').textContent = 'Cửa đã khóa';
    }
}

// Show keyboard overlay
function showKeyboardOverlay() {
    document.getElementById('keyboard-overlay').classList.add('visible');
}

// Toggle menu
function toggleMenu() {
    document.getElementById('menu').classList.toggle('visible');
}

// Close keyboard overlay
document.addEventListener('DOMContentLoaded', function() {
    document.getElementById('close-keyboard').addEventListener('click', function() {
        document.getElementById('keyboard-overlay').classList.remove('visible');
    });
});

// Simulation loop
setInterval(function() {
    // Update speed based on acceleration and braking
    if (accelerating && !handbrakeActive) {
        currentSpeed += 2;
    } else if (braking) {
        currentSpeed -= 5;
    } else {
        currentSpeed -= 0.5;
    }
    
    // Limit speed
    currentSpeed = Math.max(0, Math.min(260, currentSpeed));
    
    // Update battery level based on speed
    if (currentSpeed > 0) {
        batteryLevel -= 0.01;
    }
    
    // Limit battery level
    batteryLevel = Math.max(0, Math.min(100, batteryLevel));
    
    // Update gauges
    updateSpeedometer(currentSpeed);
    updateBatteryGauge(batteryLevel);
    
    // Update range
    document.getElementById('battery-range').textContent = Math.round(batteryLevel * 5);
    document.getElementById('estimated-range').textContent = Math.round(batteryLevel * 5);
    
}, 100);

// Initialize loading screen
document.addEventListener('DOMContentLoaded', function() {
    const loadingBar = document.getElementById('loading-bar');
    const loadingScreen = document.getElementById('loading-screen');
    
    let progress = 0;
    const interval = setInterval(function() {
        progress += 5;
        loadingBar.style.width = progress + '%';
        
        if (progress >= 100) {
            clearInterval(interval);
            setTimeout(function() {
                loadingScreen.classList.add('hidden');
            }, 500);
        }
    }, 100);
});

// Initialize gear display
document.addEventListener('DOMContentLoaded', function() {
    // Set initial gear to P
    setGear('p');
    
    // Add click event listeners to gears
    const gears = ['p', 'r', 'n', 'd'];
    gears.forEach(gear => {
        document.getElementById(`gear-${gear}`).addEventListener('click', function() {
            setGear(gear);
        });
    });
    
    // Add click event listeners to handbrake and door lock
    document.getElementById('handbrake').addEventListener('click', toggleHandbrake);
    document.getElementById('door-lock').addEventListener('click', toggleDoorLock);
});

// Add 3D effect on mouse move
document.addEventListener('mousemove', function(event) {
    const container = document.querySelector('.container');
    const gauges = document.querySelector('.analog-gauges');
    const digital = document.querySelector('.digital-dashboard');
    
    if (!container || !gauges || !digital) return;
    
    const containerRect = container.getBoundingClientRect();
    
    // Calculate mouse position relative to container center
    const centerX = containerRect.left + containerRect.width / 2;
    const centerY = containerRect.top + containerRect.height / 2;
    
    const mouseX = event.clientX;
    const mouseY = event.clientY;
    
    const offsetX = (mouseX - centerX) / 50;
    const offsetY = (mouseY - centerY) / 50;
    
    // Apply subtle 3D transform to gauges and digital dashboard
    gauges.style.transform = `perspective(1000px) rotateX(${-offsetY}deg) rotateY(${offsetX}deg)`;
    digital.style.transform = `perspective(1000px) rotateX(${-offsetY}deg) rotateY(${offsetX}deg)`;
});

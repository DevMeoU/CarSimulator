// Simulation JavaScript
document.addEventListener('DOMContentLoaded', function() {
    // Initialize simulation
    initializeSimulation();
    
    // Start simulation loop
    startSimulationLoop();
});

// Initialize simulation
function initializeSimulation() {
    // Set initial values
    window.simulationState = {
        currentSpeed: 0,
        targetSpeed: 0,
        batteryLevel: 60,
        batteryDrainRate: 0.01,
        accelerating: false,
        braking: false,
        handbrakeActive: true,
        doorLocked: true,
        currentGear: 'p',
        driveMode: 'eco',
        temperature: 25,
        batteryTemperature: 31,
        acTemperature: 22,
        fanSpeed: 1,
        estimatedRange: 300,
        totalDistance: 10015,
        tripDistance: 125,
        engineRunning: false,
        warnings: []
    };
    
    // Initialize gauges
    initializeGauges();
    
    // Initialize digital display
    initializeDigitalDisplay();
}

// Initialize gauges
function initializeGauges() {
    // Create speedometer markings
    createSpeedometerMarkings();
    
    // Create battery gauge markings
    createBatteryGaugeMarkings();
    
    // Set initial needle positions
    updateSpeedometerNeedle(0);
    updateBatteryGaugeNeedle(60);
}

// Create speedometer markings
function createSpeedometerMarkings() {
    const markingsElement = document.getElementById('speed-markings');
    const numbersElement = document.getElementById('speed-numbers');
    
    if (!markingsElement || !numbersElement) return;
    
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

// Create battery gauge markings
function createBatteryGaugeMarkings() {
    const markingsElement = document.getElementById('battery-markings');
    const numbersElement = document.getElementById('battery-numbers');
    
    if (!markingsElement || !numbersElement) return;
    
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

// Initialize digital display
function initializeDigitalDisplay() {
    // Set initial temperature
    document.getElementById('digital-temperature').textContent = `${window.simulationState.temperature.toFixed(1)}°C`;
    
    // Set initial time
    updateDigitalTime();
    
    // Set initial date
    updateDigitalDate();
    
    // Set initial drive mode
    setActiveDriveMode(window.simulationState.driveMode);
    
    // Set initial digital speed
    document.getElementById('digital-speed').textContent = '0';
    
    // Set initial battery status
    updateBatteryStatus(window.simulationState.batteryLevel);
}

// Update digital time
function updateDigitalTime() {
    const now = new Date();
    const hours = now.getHours().toString().padStart(2, '0');
    const minutes = now.getMinutes().toString().padStart(2, '0');
    document.getElementById('digital-time').textContent = `${hours}:${minutes}`;
    document.getElementById('time').textContent = `${hours}:${minutes}`;
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

// Set active drive mode
function setActiveDriveMode(mode) {
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
}

// Update battery status
function updateBatteryStatus(level) {
    const batteryStatusIcon = document.getElementById('status-battery');
    
    if (batteryStatusIcon) {
        if (level < 20) {
            batteryStatusIcon.className = 'status-icon inactive';
            addWarning('Pin yếu');
        } else {
            batteryStatusIcon.className = 'status-icon active';
        }
    }
}

// Start simulation loop
function startSimulationLoop() {
    setInterval(updateSimulation, 50);
    setInterval(updateDigitalTime, 1000);
    setInterval(updateDigitalDate, 60000);
}

// Update simulation
function updateSimulation() {
    const state = window.simulationState;
    
    // Update speed based on inputs
    updateSpeed();
    
    // Update battery level
    updateBattery();
    
    // Update temperatures
    updateTemperatures();
    
    // Update displays
    updateDisplays();
    
    // Check for warnings
    checkWarnings();
}

// Update speed
function updateSpeed() {
    const state = window.simulationState;
    
    // Calculate target speed based on inputs
    if (state.accelerating && !state.handbrakeActive && state.currentGear === 'd') {
        // Acceleration rate depends on drive mode
        let accelerationRate = 1; // Default
        
        switch (state.driveMode) {
            case 'sport':
                accelerationRate = 2;
                break;
            case 'eco':
                accelerationRate = 0.7;
                break;
            case 'classic':
                accelerationRate = 1;
                break;
        }
        
        state.targetSpeed += accelerationRate;
    } else if (state.braking) {
        state.targetSpeed -= 3;
    } else {
        // Natural deceleration
        if (state.targetSpeed > 0) {
            state.targetSpeed -= 0.3;
        }
    }
    
    // Apply handbrake
    if (state.handbrakeActive && state.targetSpeed > 0) {
        state.targetSpeed -= 5;
    }
    
    // Limit target speed
    state.targetSpeed = Math.max(0, Math.min(260, state.targetSpeed));
    
    // Smooth speed changes
    const speedDiff = state.targetSpeed - state.currentSpeed;
    state.currentSpeed += speedDiff * 0.1;
    
    // Update speedometer
    updateSpeedometerNeedle(state.currentSpeed);
    document.getElementById('speed-value').textContent = Math.round(state.currentSpeed);
    document.getElementById('digital-speed').textContent = Math.round(state.currentSpeed);
}

// Update battery
function updateBattery() {
    const state = window.simulationState;
    
    // Drain battery based on speed and drive mode
    if (state.currentSpeed > 0) {
        let drainMultiplier = 1;
        
        switch (state.driveMode) {
            case 'sport':
                drainMultiplier = 1.5;
                break;
            case 'eco':
                drainMultiplier = 0.7;
                break;
            case 'classic':
                drainMultiplier = 1;
                break;
        }
        
        // Higher speeds drain battery faster
        const speedFactor = 1 + (state.currentSpeed / 100);
        
        state.batteryLevel -= state.batteryDrainRate * drainMultiplier * speedFactor;
    }
    
    // Limit battery level
    state.batteryLevel = Math.max(0, Math.min(100, state.batteryLevel));
    
    // Update battery gauge
    updateBatteryGaugeNeedle(state.batteryLevel);
    document.getElementById('battery-value').textContent = Math.round(state.batteryLevel);
    document.getElementById('battery-percent').textContent = Math.round(state.batteryLevel) + '%';
    document.getElementById('battery-fill').style.width = state.batteryLevel + '%';
    
    // Update estimated range
    const range = Math.round(state.batteryLevel * 5);
    document.getElementById('battery-range').textContent = range;
    document.getElementById('estimated-range').textContent = range;
}

// Update temperatures
function updateTemperatures() {
    const state = window.simulationState;
    
    // Battery temperature increases with speed
    if (state.currentSpeed > 0) {
        state.batteryTemperature += 0.001 * state.currentSpeed;
    } else {
        // Battery cools down when not in use
        if (state.batteryTemperature > state.temperature) {
            state.batteryTemperature -= 0.01;
        }
    }
    
    // Limit battery temperature
    state.batteryTemperature = Math.max(state.temperature, Math.min(80, state.batteryTemperature));
    
    // Update temperature displays
    document.getElementById('battery-temp').textContent = Math.round(state.batteryTemperature);
    document.getElementById('temperature').textContent = Math.round(state.temperature);
    document.getElementById('digital-temperature').textContent = `${state.temperature.toFixed(1)}°C`;
}

// Update displays
function updateDisplays() {
    const state = window.simulationState;
    
    // Update trip distance
    if (state.currentSpeed > 0) {
        // Convert km/h to km per frame (assuming 20 frames per second)
        const distancePerFrame = state.currentSpeed / (3600 * 20);
        state.tripDistance += distancePerFrame;
        state.totalDistance += distancePerFrame;
    }
}

// Check for warnings
function checkWarnings() {
    const state = window.simulationState;
    const warningsElement = document.getElementById('warnings');
    
    if (!warningsElement) return;
    
    // Clear previous warnings
    warningsElement.innerHTML = '';
    
    // Check battery level
    if (state.batteryLevel < 20) {
        addWarningElement(warningsElement, 'Pin yếu');
    }
    
    // Check battery temperature
    if (state.batteryTemperature > 60) {
        addWarningElement(warningsElement, 'Nhiệt độ pin cao');
    }
    
    // Check if door is unlocked while moving
    if (!state.doorLocked && state.currentSpeed > 0) {
        addWarningElement(warningsElement, 'Cửa chưa khóa');
    }
    
    // Check if handbrake is active while moving
    if (state.handbrakeActive && state.currentSpeed > 0) {
        addWarningElement(warningsElement, 'Phanh tay đang kéo');
    }
}

// Add warning element
function addWarningElement(container, message) {
    const warningElement = document.createElement('div');
    warningElement.className = 'warning';
    warningElement.textContent = message;
    container.appendChild(warningElement);
}

// Update speedometer needle
function updateSpeedometerNeedle(speed) {
    const needle = document.getElementById('speed-needle');
    
    if (needle) {
        // Calculate needle angle (-135 to 135 degrees for 0 to 260 km/h)
        const angle = -135 + (speed * 270 / 260);
        
        // Update needle position
        needle.style.transform = `translate(-50%, -100%) rotate(${angle}deg)`;
    }
}

// Update battery gauge needle
function updateBatteryGaugeNeedle(percentage) {
    const needle = document.getElementById('battery-needle');
    
    if (needle) {
        // Calculate needle angle (-135 to 135 degrees for 0 to 100%)
        const angle = -135 + (percentage * 270 / 100);
        
        // Update needle position
        needle.style.transform = `translate(-50%, -100%) rotate(${angle}deg)`;
    }
}

// Add warning
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

// 3D effect on mouse move
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

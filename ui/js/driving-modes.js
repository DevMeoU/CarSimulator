// ECO/SPORT Driving Modes Implementation
document.addEventListener('DOMContentLoaded', function() {
    // Get mode buttons
    const ecoModeButton = document.getElementById('eco-mode');
    const sportModeButton = document.getElementById('sport-mode');
    
    // Get speedometer elements
    const speedFill = document.getElementById('speed-fill');
    const speedValue = document.getElementById('speed-value');
    
    // Get battery elements
    const batteryLevel = document.getElementById('battery-level');
    
    // Current mode and speed
    let currentMode = 'eco';
    let currentSpeed = 0;
    let maxSpeed = 180; // Default max speed for ECO mode
    let accelerationFactor = 5; // Default acceleration for ECO mode
    let batteryConsumptionRate = 0.05; // Default battery consumption for ECO mode
    let currentBatteryLevel = 60; // Starting battery level (%)
    
    // Initialize mode
    setDrivingMode('eco');
    
    // Mode button event listeners
    if (ecoModeButton) {
        ecoModeButton.addEventListener('click', function() {
            setDrivingMode('eco');
            showButtonPress('ECO', 'BẬT', '#4cd964');
        });
    }
    
    if (sportModeButton) {
        sportModeButton.addEventListener('click', function() {
            setDrivingMode('sport');
            showButtonPress('SPORT', 'BẬT', '#ff3b30');
        });
    }
    
    // Set driving mode
    function setDrivingMode(mode) {
        currentMode = mode;
        
        // Update UI
        if (ecoModeButton && sportModeButton) {
            if (mode === 'eco') {
                ecoModeButton.classList.add('active');
                sportModeButton.classList.remove('active');
                maxSpeed = 180;
                accelerationFactor = 5;
                batteryConsumptionRate = 0.05;
                
                // Update status display
                updateStatusDisplay('Chế độ ECO đã kích hoạt');
            } else {
                ecoModeButton.classList.remove('active');
                sportModeButton.classList.add('active');
                maxSpeed = 250;
                accelerationFactor = 10;
                batteryConsumptionRate = 0.1;
                
                // Update status display
                updateStatusDisplay('Chế độ SPORT đã kích hoạt');
            }
        }
        
        // Log mode change
        console.log(`Driving mode changed to: ${mode.toUpperCase()}`);
        console.log(`Max speed: ${maxSpeed} km/h`);
        console.log(`Acceleration factor: ${accelerationFactor}`);
        console.log(`Battery consumption rate: ${batteryConsumptionRate}%/s`);
    }
    
    // Keyboard controls for driving modes
    document.addEventListener('keydown', function(event) {
        if (event.key === 'e' || event.key === 'E') {
            setDrivingMode('eco');
            showButtonPress('ECO', 'BẬT', '#4cd964');
        } else if (event.key === 's' || event.key === 'S') {
            setDrivingMode('sport');
            showButtonPress('SPORT', 'BẬT', '#ff3b30');
        }
    });
    
    // Acceleration and deceleration controls
    let accelerating = false;
    let braking = false;
    
    document.addEventListener('keydown', function(event) {
        if (event.key === 'w' || event.key === 'W' || event.key === 'ArrowUp') {
            accelerating = true;
        } else if (event.key === 's' || event.key === 'S' || event.key === 'ArrowDown') {
            braking = true;
        }
    });
    
    document.addEventListener('keyup', function(event) {
        if (event.key === 'w' || event.key === 'W' || event.key === 'ArrowUp') {
            accelerating = false;
        } else if (event.key === 's' || event.key === 'S' || event.key === 'ArrowDown') {
            braking = false;
        }
    });
    
    // Update speed and battery level
    function updateSimulation() {
        // Update speed based on acceleration/braking
        if (accelerating) {
            currentSpeed = Math.min(currentSpeed + accelerationFactor, maxSpeed);
            updateStatusDisplay(`Đang tăng tốc: ${currentSpeed} km/h`);
        } else if (braking) {
            currentSpeed = Math.max(currentSpeed - accelerationFactor * 2, 0);
            updateStatusDisplay(`Đang phanh: ${currentSpeed} km/h`);
        } else {
            // Natural deceleration
            currentSpeed = Math.max(currentSpeed - 1, 0);
            if (currentSpeed === 0) {
                updateStatusDisplay('Xe đứng yên');
            } else {
                updateStatusDisplay(`Đang di chuyển: ${currentSpeed} km/h`);
            }
        }
        
        // Update battery level based on speed and mode
        if (currentSpeed > 0) {
            // Higher consumption at higher speeds
            const speedFactor = currentSpeed / maxSpeed;
            const consumption = batteryConsumptionRate * (0.5 + speedFactor * 0.5);
            currentBatteryLevel = Math.max(currentBatteryLevel - consumption, 0);
        }
        
        // Update UI
        updateSpeedometer(currentSpeed);
        updateBatteryDisplay(currentBatteryLevel);
        
        // Schedule next update
        requestAnimationFrame(updateSimulation);
    }
    
    // Update speedometer display
    function updateSpeedometer(speed) {
        if (speedValue) {
            speedValue.textContent = Math.round(speed);
        }
        
        if (speedFill) {
            const percentage = (speed / maxSpeed) * 100;
            speedFill.style.width = `${percentage}%`;
            
            // Change color based on speed
            if (percentage < 30) {
                speedFill.style.background = 'linear-gradient(to right, #4cd964 0%, #4cd964 100%)';
            } else if (percentage < 70) {
                speedFill.style.background = 'linear-gradient(to right, #4cd964 0%, #ff9500 100%)';
            } else {
                speedFill.style.background = 'linear-gradient(to right, #ff9500 0%, #ff3b30 100%)';
            }
        }
    }
    
    // Update battery display
    function updateBatteryDisplay(level) {
        if (batteryLevel) {
            batteryLevel.style.height = `${level}%`;
            
            // Change color based on level
            if (level > 50) {
                batteryLevel.style.background = 'linear-gradient(to top, #4cd964 0%, #4cd964 100%)';
            } else if (level > 20) {
                batteryLevel.style.background = 'linear-gradient(to top, #ff9500 0%, #ff9500 100%)';
            } else {
                batteryLevel.style.background = 'linear-gradient(to top, #ff3b30 0%, #ff3b30 100%)';
                
                // Flash battery warning if below 10%
                if (level < 10) {
                    batteryLevel.classList.add('glow-effect');
                    
                    // Show warning every 10 seconds if below 10%
                    if (Math.floor(Date.now() / 1000) % 10 === 0) {
                        showButtonPress('PIN YẾU', 'CẢNH BÁO', '#ff3b30');
                    }
                } else {
                    batteryLevel.classList.remove('glow-effect');
                }
            }
            
            // Update battery percentage display
            const batteryPercentage = document.querySelector('.battery-percentage');
            if (batteryPercentage) {
                batteryPercentage.textContent = `${Math.round(level)}%`;
            }
            
            // Update range based on battery level
            const rangeValue = document.querySelector('.range-value');
            if (rangeValue) {
                // Calculate range: 400km at 100% battery
                const range = Math.round(4 * level);
                rangeValue.textContent = `${range} km`;
            }
        }
    }
    
    // Update status display
    function updateStatusDisplay(message) {
        const statusDisplay = document.getElementById('status');
        if (statusDisplay) {
            statusDisplay.textContent = message;
        }
    }
    
    // Show button press notification
    function showButtonPress(action, state, color) {
        // Create button press element if it doesn't exist
        let buttonPress = document.querySelector('.button-press');
        if (!buttonPress) {
            buttonPress = document.createElement('div');
            buttonPress.className = 'button-press';
            document.body.appendChild(buttonPress);
            
            const iconDiv = document.createElement('div');
            iconDiv.className = 'button-press-icon';
            buttonPress.appendChild(iconDiv);
            
            const textDiv = document.createElement('div');
            textDiv.className = 'button-press-text';
            buttonPress.appendChild(textDiv);
        }
        
        // Update content
        const iconDiv = buttonPress.querySelector('.button-press-icon');
        const textDiv = buttonPress.querySelector('.button-press-text');
        
        iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
            <path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm4.59-12.42L10 14.17l-2.59-2.58L6 13l4 4 8-8z" fill="currentColor"/>
        </svg>`;
        
        textDiv.textContent = `${action}: ${state}`;
        
        // Set color
        buttonPress.style.borderLeft = `4px solid ${color}`;
        
        // Show and animate
        buttonPress.classList.remove('visible');
        void buttonPress.offsetWidth; // Trigger reflow
        buttonPress.classList.add('visible');
        
        // Remove after animation
        setTimeout(() => {
            buttonPress.classList.remove('visible');
        }, 2000);
    }
    
    // Start simulation
    updateSimulation();
});

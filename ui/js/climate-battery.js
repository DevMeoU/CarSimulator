// Battery and Climate Control Displays Implementation
document.addEventListener('DOMContentLoaded', function() {
    // Get battery elements
    const batteryLevel = document.getElementById('battery-level');
    const batteryPercentage = document.querySelector('.battery-percentage');
    const rangeValue = document.querySelector('.range-value');
    
    // Get temperature elements
    const cabinTempValue = document.querySelector('.temp-value:nth-of-type(1)');
    const outsideTempValue = document.querySelector('.temp-value:nth-of-type(2)');
    const acValue = document.querySelector('.ac-value');
    const fanBars = document.querySelectorAll('.fan-bar');
    
    // Initial values
    let currentBatteryLevel = 60; // Starting battery level (%)
    let cabinTemp = 22; // Starting cabin temperature (°C)
    let outsideTemp = 31; // Starting outside temperature (°C)
    let acTemp = 24; // Starting AC temperature (°C)
    let fanLevel = 2; // Starting fan level (1-4)
    
    // Initialize displays
    updateBatteryDisplay(currentBatteryLevel);
    updateTemperatureDisplay();
    
    // Battery simulation
    function simulateBattery() {
        // Random small fluctuations in battery level
        const fluctuation = (Math.random() - 0.5) * 0.1;
        currentBatteryLevel = Math.max(0, Math.min(100, currentBatteryLevel + fluctuation));
        
        // Update battery display
        updateBatteryDisplay(currentBatteryLevel);
        
        // Schedule next update
        setTimeout(simulateBattery, 5000);
    }
    
    // Temperature simulation
    function simulateTemperature() {
        // Random small fluctuations in outside temperature
        const outsideFluctuation = (Math.random() - 0.5) * 0.2;
        outsideTemp = Math.round((outsideTemp + outsideFluctuation) * 10) / 10;
        
        // Cabin temperature gradually approaches AC temperature
        if (Math.abs(cabinTemp - acTemp) > 0.1) {
            cabinTemp += (acTemp - cabinTemp) * 0.1;
            cabinTemp = Math.round(cabinTemp * 10) / 10;
        }
        
        // Update temperature display
        updateTemperatureDisplay();
        
        // Schedule next update
        setTimeout(simulateTemperature, 3000);
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
                        showNotification('PIN YẾU', 'CẢNH BÁO', '#ff3b30');
                    }
                } else {
                    batteryLevel.classList.remove('glow-effect');
                }
            }
            
            // Update battery percentage display
            if (batteryPercentage) {
                batteryPercentage.textContent = `${Math.round(level)}%`;
            }
            
            // Update range based on battery level
            if (rangeValue) {
                // Calculate range: 400km at 100% battery
                const range = Math.round(4 * level);
                rangeValue.textContent = `${range} km`;
            }
        }
    }
    
    // Update temperature display
    function updateTemperatureDisplay() {
        if (cabinTempValue) {
            cabinTempValue.textContent = `${cabinTemp}°C`;
        }
        
        if (outsideTempValue) {
            outsideTempValue.textContent = `${outsideTemp}°C`;
        }
        
        if (acValue) {
            acValue.textContent = `${acTemp}°C`;
        }
        
        // Update fan level bars
        if (fanBars && fanBars.length > 0) {
            for (let i = 0; i < fanBars.length; i++) {
                if (i < fanLevel) {
                    fanBars[i].classList.add('active');
                } else {
                    fanBars[i].classList.remove('active');
                }
            }
        }
    }
    
    // Climate control keyboard shortcuts
    document.addEventListener('keydown', function(event) {
        // Temperature up/down
        if (event.key === '+' || event.key === '=') {
            acTemp = Math.min(acTemp + 1, 30);
            updateTemperatureDisplay();
            showNotification('ĐIỀU HÒA', `${acTemp}°C`, '#007aff');
        } else if (event.key === '-' || event.key === '_') {
            acTemp = Math.max(acTemp - 1, 16);
            updateTemperatureDisplay();
            showNotification('ĐIỀU HÒA', `${acTemp}°C`, '#007aff');
        }
        
        // Fan level up/down
        if (event.key === ']' || event.key === '}') {
            fanLevel = Math.min(fanLevel + 1, 4);
            updateTemperatureDisplay();
            showNotification('QUẠT GIÓ', `Mức ${fanLevel}`, '#007aff');
        } else if (event.key === '[' || event.key === '{') {
            fanLevel = Math.max(fanLevel - 1, 0);
            updateTemperatureDisplay();
            showNotification('QUẠT GIÓ', `Mức ${fanLevel}`, '#007aff');
        }
    });
    
    // Add click handlers for temperature controls
    const tempControls = document.querySelectorAll('.temperature-control');
    if (tempControls && tempControls.length >= 2) {
        // Left temperature control (decrease)
        tempControls[0].addEventListener('click', function() {
            acTemp = Math.max(acTemp - 1, 16);
            updateTemperatureDisplay();
            showNotification('ĐIỀU HÒA', `${acTemp}°C`, '#007aff');
        });
        
        // Right temperature control (increase)
        tempControls[1].addEventListener('click', function() {
            acTemp = Math.min(acTemp + 1, 30);
            updateTemperatureDisplay();
            showNotification('ĐIỀU HÒA', `${acTemp}°C`, '#007aff');
        });
    }
    
    // Show notification
    function showNotification(title, message, color) {
        // Create notification element if it doesn't exist
        let notification = document.querySelector('.button-press');
        if (!notification) {
            notification = document.createElement('div');
            notification.className = 'button-press';
            document.body.appendChild(notification);
            
            const iconDiv = document.createElement('div');
            iconDiv.className = 'button-press-icon';
            notification.appendChild(iconDiv);
            
            const textDiv = document.createElement('div');
            textDiv.className = 'button-press-text';
            notification.appendChild(textDiv);
        }
        
        // Update content
        const iconDiv = notification.querySelector('.button-press-icon');
        const textDiv = notification.querySelector('.button-press-text');
        
        // Set icon based on notification type
        if (title === 'ĐIỀU HÒA' || title === 'QUẠT GIÓ') {
            iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
                <path d="M22 11h-4.17l3.24-3.24-1.41-1.42L15 11h-2V9l4.66-4.66-1.42-1.41L13 6.17V2h-2v4.17L7.76 2.93 6.34 4.34 11 9v2H9L4.34 6.34 2.93 7.76 6.17 11H2v2h4.17l-3.24 3.24 1.41 1.42L9 13h2v2l-4.66 4.66 1.42 1.41L11 17.83V22h2v-4.17l3.24 3.24 1.42-1.41L13 15v-2h2l4.66 4.66 1.41-1.42L17.83 13H22z" fill="currentColor"/>
            </svg>`;
        } else if (title === 'PIN YẾU') {
            iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
                <path d="M15.67 4H14V2h-4v2H8.33C7.6 4 7 4.6 7 5.33v15.33C7 21.4 7.6 22 8.33 22h7.33c.74 0 1.34-.6 1.34-1.33V5.33C17 4.6 16.4 4 15.67 4zM13 18h-2v-2h2v2zm0-4h-2V9h2v5z" fill="currentColor"/>
            </svg>`;
        } else {
            iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
                <path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm4.59-12.42L10 14.17l-2.59-2.58L6 13l4 4 8-8z" fill="currentColor"/>
            </svg>`;
        }
        
        textDiv.textContent = `${title}: ${message}`;
        
        // Set color
        notification.style.borderLeft = `4px solid ${color}`;
        
        // Show and animate
        notification.classList.remove('visible');
        void notification.offsetWidth; // Trigger reflow
        notification.classList.add('visible');
        
        // Remove after animation
        setTimeout(() => {
            notification.classList.remove('visible');
        }, 2000);
    }
    
    // Start simulations
    simulateBattery();
    simulateTemperature();
    
    // Battery charging simulation
    let charging = false;
    
    // Toggle charging with 'c' key
    document.addEventListener('keydown', function(event) {
        if (event.key === 'c' || event.key === 'C') {
            charging = !charging;
            
            if (charging) {
                showNotification('SẠC PIN', 'BẬT', '#4cd964');
                
                // Start charging animation
                startCharging();
            } else {
                showNotification('SẠC PIN', 'TẮT', '#ff3b30');
                
                // Stop charging animation
                stopCharging();
            }
        }
    });
    
    // Charging animation
    let chargingInterval;
    
    function startCharging() {
        // Clear any existing interval
        if (chargingInterval) {
            clearInterval(chargingInterval);
        }
        
        // Start charging animation
        chargingInterval = setInterval(() => {
            if (currentBatteryLevel < 100) {
                currentBatteryLevel = Math.min(100, currentBatteryLevel + 0.5);
                updateBatteryDisplay(currentBatteryLevel);
            } else {
                // Battery full
                showNotification('PIN ĐẦY', '100%', '#4cd964');
                stopCharging();
            }
        }, 1000);
    }
    
    function stopCharging() {
        charging = false;
        if (chargingInterval) {
            clearInterval(chargingInterval);
            chargingInterval = null;
        }
    }
});
